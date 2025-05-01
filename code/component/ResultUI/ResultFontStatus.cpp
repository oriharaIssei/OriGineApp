#include "ResultFontStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"
#include"component/ResultUI/ResultUIParentStatus.h"

void ResultFontStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ResultFontStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat3("initPos", initPos_.v, 0.01f);
    isChange |= ImGui::DragFloat3("easePos", easePos_.v, 0.01f);
    isChange |= ImGui::DragFloat2("easeScale", easeScale_.v, 0.01f);

    ImGui::Text("moveEasing");
    isChange |= ImGui::DragFloat("moveEasing.maxTime", &moveEasing_.maxTime, 0.01f);
    ImGui::Text("scaleEasing");
    isChange |= ImGui::DragFloat("zoomEasing_.maxTime", &zoomEasing_.maxTime, 0.01f);

    ImGui::Text("waitTime");
    isChange |= ImGui::DragFloat("reverseWaitTime_", &moveWaitTime_, 0.01f);
    isChange |= ImGui::DragFloat("firstWaitTime_", &firstWaitTime_, 0.01f);

    return isChange;
}

void ResultFontStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("moveEasing.maxTime", moveEasing_.maxTime);
    _writer.Write<3, float>("easePos", easePos_);
    _writer.Write("zoomEasing_.maxTime", zoomEasing_.maxTime);
    _writer.Write("scoreWaitTime", moveWaitTime_);
    _writer.Write<3, float>("initPos", initPos_);
    _writer.Write("zoomValue", zoomValue_);
    _writer.Write("reverseWaitTime", moveWaitTime_);
    _writer.Write("firstWaitTime", firstWaitTime_);
    _writer.Write<2, float>("easeScale", easeScale_);
}

void ResultFontStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("moveEasing.maxTime", moveEasing_.maxTime);
    _reader.Read<3, float>("easePos", easePos_);
    _reader.Read("zoomEasing_.maxTime", zoomEasing_.maxTime);
    _reader.Read("scoreWaitTime", moveWaitTime_);
    _reader.Read<3, float>("initPos", initPos_);
    _reader.Read("zoomValue", zoomValue_);
    _reader.Read("reverseWaitTime", moveWaitTime_);
    _reader.Read("firstWaitTime", firstWaitTime_);
    _reader.Read<2, float>("easeScale", easeScale_);
}

void ResultFontStatus::Finalize() {}

void ResultFontStatus::ZoomAnimation(const float& time) {
    zoomEasing_.time += time;
    baseScale_ = EaseOutBack(Vec2f(0.0f, 0.0f), easeScale_, zoomEasing_.time, zoomEasing_.maxTime);

    if (zoomEasing_.time < zoomEasing_.maxTime) {
        return;
    }

    baseScale_       = easeScale_;
    zoomEasing_.time = zoomEasing_.maxTime;
    curerntStep_     = ResultFontStep::MOVEWAIT;
}

void ResultFontStatus::MoveAnimation(const float& time, ResultUIParentStatus& parentStatus) {

    moveEasing_.time += time;

    basePos_   = EaseOutBack(initPos_, easePos_, moveEasing_.time, moveEasing_.maxTime);
    baseScale_ = EaseInCirc(easeScale_, Vec2f(1.0f, 1.0f), moveEasing_.time, moveEasing_.maxTime);

    if (moveEasing_.time >= moveEasing_.maxTime - 0.6f) {
        parentStatus.SetIsAnimation(true);
    }

    if (moveEasing_.time < moveEasing_.maxTime) {
        return;
    }

    moveEasing_.time = moveEasing_.maxTime;
    basePos_         = easePos_;
    baseScale_       = Vec2f(1.0f, 1.0f);
    curerntStep_     = ResultFontStep::END;
}

void ResultFontStatus::Reset() {
    moveEasing_.time = 0.0f;
    zoomEasing_.time = 0.0f;
    basePos_         = initPos_;
}
