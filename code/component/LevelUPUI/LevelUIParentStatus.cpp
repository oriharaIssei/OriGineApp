#include "LevelUIParentStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void to_json(nlohmann::json& j, const LevelUIParentStatus& l) {
    j["isAlive"]           = l.isAlive_;
    j["initPos"]           = l.initPos_;
    j["easePos"]           = l.easePos_;
    j["easeScale"]         = l.easeScale_;
    j["changingEaseScale"] = l.changingEaseScale_;
    j["moveOffset"]        = l.moveOffset_;
    j["scrollWaitTime"]    = l.scrollWaitTime_;
    j["reverseWaitTime"]   = l.reverseWaitTime_;

    j["moveEasing.maxTime"]     = l.moveEasing_.maxTime;
    j["scaleEasing.maxTime"]    = l.scaleEasing_.maxTime;
    j["scaleEasing.amplitude"]  = l.scaleEasing_.amplitude;
    j["scaleEasing.period"]     = l.scaleEasing_.period;
    j["scaleEasing.backRatio"]  = l.scaleEasing_.backRatio;
    j["uvScrollEasing.maxTime"] = l.uvScrollEasing_.maxTime;
}
void from_json(const nlohmann::json& j, LevelUIParentStatus& l) {
    j.at("isAlive").get_to(l.isAlive_);
    j.at("initPos").get_to(l.initPos_);
    j.at("easePos").get_to(l.easePos_);
    j.at("easeScale").get_to(l.easeScale_);
    j.at("changingEaseScale").get_to(l.changingEaseScale_);
    j.at("moveOffset").get_to(l.moveOffset_);
    j.at("scrollWaitTime").get_to(l.scrollWaitTime_);
    j.at("reverseWaitTime").get_to(l.reverseWaitTime_);

    j.at("moveEasing.maxTime").get_to(l.moveEasing_.maxTime);
    j.at("scaleEasing.maxTime").get_to(l.scaleEasing_.maxTime);
    j.at("scaleEasing.amplitude").get_to(l.scaleEasing_.amplitude);
    j.at("scaleEasing.period").get_to(l.scaleEasing_.period);
    j.at("scaleEasing.backRatio").get_to(l.scaleEasing_.backRatio);
    j.at("uvScrollEasing.maxTime").get_to(l.uvScrollEasing_.maxTime);
}

void LevelUIParentStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    currentLevelUV_ = 0;
}

bool LevelUIParentStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat3("initPos", initPos_.v);
    isChange |= ImGui::DragFloat3("easePos", easePos_.v);
    isChange |= ImGui::DragFloat2("easeScale", easeScale_.v);
    isChange |= ImGui::DragFloat2("changingEaseScale", changingEaseScale_.v);

    isChange |= ImGui::DragFloat3("moveOffset", moveOffset_.v);

    ImGui::Text("moveEasing");
    isChange |= ImGui::DragFloat("moveEasing.maxTime", &moveEasing_.maxTime, 0.01f);
    ImGui::Text("scaleEasing");
    isChange |= ImGui::DragFloat("scaleEasing.maxTime", &scaleEasing_.maxTime, 0.01f);
    isChange |= ImGui::DragFloat("scaleEasing.amplitude", &scaleEasing_.amplitude, 0.01f);
    isChange |= ImGui::DragFloat("scaleEasing.period", &scaleEasing_.period, 0.01f);
    isChange |= ImGui::DragFloat("scaleEasing.backRatio", &scaleEasing_.backRatio, 0.01f);
    ImGui::Text("uvScrollEasing");
    isChange |= ImGui::DragFloat("uvScrollEasing.maxTime", &uvScrollEasing_.maxTime, 0.01f);

    ImGui::Text("waitTime");
    isChange |= ImGui::DragFloat("scrollWaitTime", &scrollWaitTime_, 0.01f);
    isChange |= ImGui::DragFloat("reverseWaitTime", &reverseWaitTime_, 0.01f);

    return isChange;
}

void LevelUIParentStatus::Finalize() {}

void LevelUIParentStatus::MoveAnimation(const float& time) {

    moveEasing_.time += time;

    basePos_           = EaseOutBack(initPos_, easePos_, moveEasing_.time, moveEasing_.maxTime);
    baseScale_         = EaseOutBack(initScale_, easeScale_, moveEasing_.time, moveEasing_.maxTime);
    currentmoveOffset_ = EaseOutBack(Vec3f(0.0f, 0.0f, 0.0f), moveOffset_, moveEasing_.time, moveEasing_.maxTime);

    if (moveEasing_.time < moveEasing_.maxTime) {
        return;
    }

    moveEasing_.time   = moveEasing_.maxTime;
    basePos_           = easePos_;
    baseScale_         = easeScale_;
    currentmoveOffset_ = moveOffset_;
    curerntStep_       = AnimationStep::SCROLLWAIT;
}

void LevelUIParentStatus::ScrollAnimation(const float& time) {
    uvScrollEasing_.time += time;

    // 現在のUVを補間計算
    currentLevelUV_ = EaseOutBack(preLevelUV_, nextLevelUV_, uvScrollEasing_.time, uvScrollEasing_.maxTime);

    if (!hasStartedScaling_ && uvScrollEasing_.time >= (uvScrollEasing_.maxTime - 0.2f)) {
        hasStartedScaling_ = true; // フラグを立てて1回だけ起動させる
        curerntStep_       = AnimationStep::SCALING; // 状態変更など必要に応じて
    }

    // UVスクロール完了時の後処理
    if (uvScrollEasing_.time < uvScrollEasing_.maxTime) {
        return;
    }

    uvScrollEasing_.time = uvScrollEasing_.maxTime;
    currentLevelUV_      = nextLevelUV_;
    preLevelUV_          = currentLevelUV_;
}

void LevelUIParentStatus::ScalingAnimation(const float& time) {
    scaleEasing_.time += time;
    baseScale_ = EaseAmplitudeScale(easeScale_, scaleEasing_.time, scaleEasing_.maxTime, scaleEasing_.amplitude, scaleEasing_.period);

    if (scaleEasing_.time < scaleEasing_.maxTime) {
        return;
    }

    baseScale_        = easeScale_;
    scaleEasing_.time = scaleEasing_.maxTime;
    curerntStep_      = AnimationStep::REVERSEWAIT;
}

void LevelUIParentStatus::ReverseAnimation(const float& time) {
    moveEasing_.time -= time;

    basePos_           = EaseInBack(initPos_, easePos_, moveEasing_.time, moveEasing_.maxTime);
    baseScale_         = EaseInBack(initScale_, easeScale_, moveEasing_.time, moveEasing_.maxTime);
    currentmoveOffset_ = EaseInBack(Vec3f(0.0f, 0.0f, 0.0f), moveOffset_, moveEasing_.time, moveEasing_.maxTime);

    if (moveEasing_.time > 0.0f) {
        return;
    }

    Reset();
    isLevelChange_ = false;
    curerntStep_   = AnimationStep::NONE;
}

void LevelUIParentStatus::Reset() {
    moveEasing_.time     = 0.0f;
    uvScrollEasing_.time = 0.0f;
    scaleEasing_.time    = 0.0f;
    basePos_             = initPos_;
    baseScale_           = initScale_;
    currentmoveOffset_   = Vec3f(0.0f, 0.0f, 0.0f);
    hasStartedScaling_   = false;
}

void LevelUIParentStatus::Init() {
    moveEasing_.time     = 0.0f;
    uvScrollEasing_.time = 0.0f;
    scaleEasing_.time    = 0.0f;
    basePos_             = initPos_;
    hasStartedScaling_   = false;
}
