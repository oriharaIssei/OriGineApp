#include "ResultUIKeyStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void ResultUIkeyStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ResultUIkeyStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat3("initPos", initPos_.v);
    isChange |= ImGui::DragFloat3("easePos", easePos_.v);
    ImGui::Text("moveEasing");
    isChange |= ImGui::DragFloat("moveEasing.maxTime", &moveEasing_.maxTime, 0.01f);

    ImGui::Text("AlphaEasing");
    isChange |= ImGui::DragFloat("alphaEasing.maxTime", &alphaEasing_.maxTime, 0.01f);
  
    return isChange;
}

void ResultUIkeyStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("moveEasing.maxTime", moveEasing_.maxTime);
    _writer.Write<3, float>("easePos", easePos_);
    _writer.Write<3, float>("initPos", initPos_);
    _writer.Write("alphaEasing.maxTime", alphaEasing_.maxTime);
}

void ResultUIkeyStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("moveEasing.maxTime", moveEasing_.maxTime);
    _reader.Read<3, float>("easePos", easePos_);
    _reader.Read<3, float>("initPos", initPos_);
    _reader.Read("alphaEasing.maxTime", alphaEasing_.maxTime);
}

void ResultUIkeyStatus::Finalize() {}

void ResultUIkeyStatus::MoveAnimation(const float& time) {

    moveEasing_.time += time;

    basePos_ = EaseInCubic(initPos_, easePos_, moveEasing_.time, moveEasing_.maxTime);
    /* scoreScale_ = EaseInCubic(scoreInitScale_, scoreEaseScale_, moveEasing_.time, moveEasing_.maxTime);*/

    if (moveEasing_.time < moveEasing_.maxTime) {
        return;
    }

    moveEasing_.time = moveEasing_.maxTime;
    basePos_         = easePos_;
    /* scoreScale_      = scoreEaseScale_;*/
    curerntStep_ = KeyResultStep::END;
}

void ResultUIkeyStatus::AlphaAnimation(const float& time) {
    alphaEasing_.time += time;

    // 現在のUVを補間計算
    alpha_ = EaseInCirc(0.0f, 1.0f, alphaEasing_.time, alphaEasing_.maxTime);

    // UVスクロール完了時の後処理
    if (alphaEasing_.time < alphaEasing_.maxTime) {
        return;
    }

    alphaEasing_.time = alphaEasing_.maxTime;
    alpha_            = 1.0f;
}

void ResultUIkeyStatus::Reset() {
    moveEasing_.time   = 0.0f;
    alphaEasing_.time  = 0.0f;
    basePos_           = initPos_;
    alpha_             = 0.0f;
}
