#include "ResultUIParentStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void to_json(nlohmann::json& _json, const ResultUIParentStatus& _component) {
    _json["isAlive"]               = _component.isAlive_;
    _json["scoreUpSpeed"]          = _component.scoreUpSpeed_;
    _json["scoreWaitTime"]         = _component.scoreWaitTime_;
    _json["moveEasing.maxTime"]    = _component.moveEasing_.maxTime;
    _json["scaleEasing.maxTime"]   = _component.scaleEasing_.maxTime;
    _json["scaleEasing.amplitude"] = _component.scaleEasing_.amplitude;
    _json["scaleEasing.period"]    = _component.scaleEasing_.period;
    _json["scaleEasing.backRatio"] = _component.scaleEasing_.backRatio;
    _json["alphaEasing.maxTime"]   = _component.alphaEasing_.maxTime;
    _json["easePos"]               = _component.easePos_;
    _json["easeScale"]             = _component.scoreEaseScale_;
}

void from_json(const nlohmann::json& _json, ResultUIParentStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    _json.at("scoreUpSpeed").get_to(_component.scoreUpSpeed_);
    _json.at("scoreWaitTime").get_to(_component.scoreWaitTime_);
    _json.at("moveEasing.maxTime").get_to(_component.moveEasing_.maxTime);
    _json.at("scaleEasing.maxTime").get_to(_component.scaleEasing_.maxTime);
    _json.at("scaleEasing.amplitude").get_to(_component.scaleEasing_.amplitude);
    _json.at("scaleEasing.period").get_to(_component.scaleEasing_.period);
    _json.at("scaleEasing.backRatio").get_to(_component.scaleEasing_.backRatio);
    _json.at("alphaEasing.maxTime").get_to(_component.alphaEasing_.maxTime);
    _json.at("easePos").get_to(_component.easePos_);
    _json.at("easeScale").get_to(_component.scoreEaseScale_);
}

void ResultUIParentStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ResultUIParentStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat3("initPos", initPos_.v);
    isChange |= ImGui::DragFloat3("easePos", easePos_.v);
    isChange |= ImGui::DragFloat2("easeScale", scoreEaseScale_.v);
    isChange |= ImGui::DragFloat("scoreUpSpeed", &scoreUpSpeed_, 0.01f);

    ImGui::Text("moveEasing");
    isChange |= ImGui::DragFloat("moveEasing.maxTime", &moveEasing_.maxTime, 0.01f);
    ImGui::Text("scaleEasing");
    isChange |= ImGui::DragFloat("scaleEasing.maxTime", &scaleEasing_.maxTime, 0.01f);
    isChange |= ImGui::DragFloat("scaleEasing.amplitude", &scaleEasing_.amplitude, 0.01f);
    isChange |= ImGui::DragFloat("scaleEasing.period", &scaleEasing_.period, 0.01f);
    isChange |= ImGui::DragFloat("scaleEasing.backRatio", &scaleEasing_.backRatio, 0.01f);
    ImGui::Text("AlphaEasing");
    isChange |= ImGui::DragFloat("alphaEasing.maxTime", &alphaEasing_.maxTime, 0.01f);
    ImGui::Text("waitTime");
    isChange |= ImGui::DragFloat("scoreWaitTime", &scoreWaitTime_, 0.01f);

    return isChange;
}

void ResultUIParentStatus::Finalize() {}

void ResultUIParentStatus::MoveAnimation(const float& time) {

    moveEasing_.time += time;

    basePos_ = EaseInCubic(initPos_, easePos_, moveEasing_.time, moveEasing_.maxTime);
    /* scoreScale_ = EaseInCubic(scoreInitScale_, scoreEaseScale_, moveEasing_.time, moveEasing_.maxTime);*/

    if (moveEasing_.time < moveEasing_.maxTime) {
        return;
    }

    moveEasing_.time = moveEasing_.maxTime;
    basePos_         = easePos_;
    /* scoreScale_      = scoreEaseScale_;*/
    curerntStep_ = ResultStep::SCOREUPWAIT;
}

void ResultUIParentStatus::AlphaAnimation(const float& time) {
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

void ResultUIParentStatus::ScoreScalingAnimation(const float& time) {
    scaleEasing_.time += time;
    scoreScale_ = Back::InCubicZero(Vec2f(1.0f, 1.0f), scoreEaseScale_, scaleEasing_.time, scaleEasing_.maxTime, scaleEasing_.backRatio);

    if (scaleEasing_.time < scaleEasing_.maxTime) {
        return;
    }

    scoreScale_       = Vec2f(1.0f, 1.0f);
    scaleEasing_.time = scaleEasing_.maxTime;
    curerntStep_      = ResultStep::END;
}

void ResultUIParentStatus::ScoreUP(const float& time) {
    currentScore_ += scoreUpSpeed_ * time;

    if (currentScore_ < resultScore_) {
        return;
    }

    currentScore_ = resultScore_;
    curerntStep_  = ResultStep::SCOREUPSCALING;
}

void ResultUIParentStatus::Reset() {
    moveEasing_.time   = 0.0f;
    scaleEasing_.time  = 0.0f;
    alphaEasing_.time  = 0.0f;
    basePos_           = initPos_;
    scoreScale_        = scoreInitScale_;
    hasStartedScaling_ = false;
}
