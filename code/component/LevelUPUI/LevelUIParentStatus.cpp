#include "LevelUIParentStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void LevelUIParentStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    currentLevel_ = 0;
}

bool LevelUIParentStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat3("easePos", easePos_.v);

    ImGui::Text("moveEasing");
    isChange |= ImGui::DragFloat("moveEasing.maxTime", &moveEasing_.maxTime);
    ImGui::Text("scaleEasing");
    isChange |= ImGui::DragFloat("scaleEasing.maxTime", &scaleEasing_.maxTime);
    isChange |= ImGui::DragFloat("scaleEasing.amplitude", &scaleEasing_.amplitude);
    isChange |= ImGui::DragFloat("scaleEasing.period", &scaleEasing_.period);
    ImGui::Text("uvScrollEasing");
    isChange |= ImGui::DragFloat("uvScrollEasing.maxTime", &uvScrollEasing_.maxTime);

    return isChange;
}

void LevelUIParentStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("moveEasing.maxTime", moveEasing_.maxTime);
    _writer.Write("scaleEasing.maxTime", scaleEasing_.maxTime);
    _writer.Write("scaleEasing.amplitude", scaleEasing_.amplitude);
    _writer.Write("scaleEasing.period", scaleEasing_.period);
    _writer.Write("uvScrollEasing.maxTime", uvScrollEasing_.maxTime);
    _writer.Write("easePos", easePos_);
}

void LevelUIParentStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("moveEasing.maxTime", moveEasing_.maxTime);
    _reader.Read("scaleEasing.maxTime", scaleEasing_.maxTime);
    _reader.Read("scaleEasing.amplitude", scaleEasing_.amplitude);
    _reader.Read("scaleEasing.period", scaleEasing_.period);
    _reader.Read("uvScrollEasing.maxTime", uvScrollEasing_.maxTime);
    _reader.Read("easePos", easePos_);
}

void LevelUIParentStatus::Finalize() {}

void LevelUIParentStatus::MoveAnimation(const float& time) {

    moveEasing_.time += time;

    basePos_   = EaseInCirc(initPos_, easePos_, moveEasing_.time, moveEasing_.maxTime);
    baseScale_ = EaseInCirc(initScale_, easeScale_, moveEasing_.time, moveEasing_.maxTime);

    if (moveEasing_.time < moveEasing_.maxTime) {
        return;
    }

    moveEasing_.time = moveEasing_.maxTime;
    basePos_         = easePos_;
    baseScale_       = easeScale_;
    curerntStep_     = AnimationStep::SCROLL;
}

void LevelUIParentStatus::ScrollAnimation(const float& time) {
    uvScrollEasing_.time += time;
    currentUVPos_ = EaseInCirc(saveUVPos_, float(currentLevel_) * 0.1f, uvScrollEasing_.time, uvScrollEasing_.maxTime);

    if (uvScrollEasing_.time < uvScrollEasing_.maxTime) {
        return;
    }

    uvScrollEasing_.time = uvScrollEasing_.maxTime;
    currentUVPos_        = float(currentLevel_) * 0.1f;
    curerntStep_         = AnimationStep::SCALING;
}

void LevelUIParentStatus::ScalingAnimation(const float& time) {
    scaleEasing_.time += time;
    baseScale_ = EaseInCirc(easeScale_, changingEaseScale_, scaleEasing_.time, scaleEasing_.maxTime);

    if (scaleEasing_.time < scaleEasing_.maxTime) {
        return;
    }

    baseScale_        = easeScale_;
    scaleEasing_.time = scaleEasing_.maxTime;
    curerntStep_      = AnimationStep::REVERSE;
}

void LevelUIParentStatus::ReverseAnimation(const float& time) {
    moveEasing_.time -= time;

    basePos_   = EaseInCirc(initPos_, easePos_, moveEasing_.time, moveEasing_.maxTime);
    baseScale_ = EaseInCirc(initScale_, easeScale_, moveEasing_.time, moveEasing_.maxTime);

    if (moveEasing_.time > 0.0f) {
        return;
    }

    Reset();
    curerntStep_ = AnimationStep::NONE;
 }

void LevelUIParentStatus::Reset() {
    moveEasing_.time     = 0.0f;
    uvScrollEasing_.time = 0.0f;
    basePos_             = initPos_;
    baseScale_           = initScale_;
    saveUVPos_           = currentUVPos_;
}
