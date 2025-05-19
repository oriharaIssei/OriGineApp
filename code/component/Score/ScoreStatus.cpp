#include "ScoreStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

float ScoreStatus::currentScore_ = 0.0f;

void ScoreStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ScoreStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiCommand("currentScore", currentScore_);
    isChange |= DragGuiCommand("pulusScore", pulusScoreValue_);
    isChange |= DragVectorGui("baseScoreTextureSize", baseScoreTextureSize_);
    isChange |= DragVectorGui("basePlusScoreTextureSize", basePlusScoreTextureSize_);
    isChange |= DragGuiCommand("waitTime_", waitTime_);

    ImGui::Text("scoreAdaptEasing");
    isChange |= DragGuiCommand("scoreAdaptEasing_.maxTime", scoreAdaptEasing_.maxTime);
    ImGui::Text("upScpreUpEasing_");
    isChange |= DragGuiCommand("upScpreUpEasing_.maxTime", upScpreUpEasing_.maxTime);
    ImGui::Text("scaleEasing_");
    isChange |= DragGuiCommand("scaleEasing_.maxTime", scaleEasing_.maxTime);
    isChange |= DragGuiCommand("scaleEasing_.backRatio", scaleEasing_.backRatio);
    isChange |= DragGuiCommand("scaleEasing_.amplitude", scaleEasing_.amplitude);
    isChange |= DragGuiCommand("scaleEasing_.period", scaleEasing_.period);
    ImGui::Text("scalingEasingBaseScore_");
    isChange |= DragGuiCommand("scalingEasingBaseScore_.maxTime", scalingEasingBaseScore_.maxTime);
    isChange |= DragGuiCommand("scalingEasingBaseScore_.backRatio", scalingEasingBaseScore_.backRatio);
    isChange |= DragGuiCommand("scalingEasingBaseScore_.amplitude", scalingEasingBaseScore_.amplitude);
    isChange |= DragGuiCommand("scalingEasingBaseScore_.period", scalingEasingBaseScore_.period);

#endif // _DEBUG

    return isChange;
}

void ScoreStatus::Finalize() {}

void ScoreStatus::PlusScoreIncrement(const float& value) {
    pulusScoreValue_ += value;
    totalScore_ += value;
}

void ScoreStatus::PlusScorePlusAnimation(const float& value) {

    upScpreUpEasing_.time += value;

    plusScore_ = EaseInOutQuint(plusScore_, pulusScoreValue_, upScpreUpEasing_.time, upScpreUpEasing_.maxTime);

    if (upScpreUpEasing_.time < upScpreUpEasing_.maxTime) {
        return;
    }
    plusScore_            = pulusScoreValue_;
    scoreUIStep_          = ScoreUIStep::WAIT;
    upScpreUpEasing_.time = upScpreUpEasing_.maxTime;
}

void ScoreStatus::ScoreUPAmplitudeScaling(const float& value) {
    scaleEasing_.time += value;

    plusScoreScale_ = EaseAmplitudeScale<Vec2f>(Vec2f(1.0f, 1.0f), scaleEasing_.time, scaleEasing_.maxTime, scaleEasing_.amplitude, scaleEasing_.period);

    if (scaleEasing_.time < scaleEasing_.maxTime) {
        return;
    }

    plusScoreScale_   = Vec2f(1.0f, 1.0f);
    scaleEasing_.time = scaleEasing_.maxTime;
}

void ScoreStatus::BaseScoreUPAmplitudeScaling(const float& value) {
    if (!isChanging_) {
        return;
    }
    scalingEasingBaseScore_.time += value;

    baseScoreScale_ = EaseAmplitudeScale<Vec2f>(Vec2f(1.0f, 1.0f), scalingEasingBaseScore_.time, scalingEasingBaseScore_.maxTime, scalingEasingBaseScore_.amplitude, scalingEasingBaseScore_.period);

    if (scalingEasingBaseScore_.time < scalingEasingBaseScore_.maxTime) {
        return;
    }

    baseScoreScale_              = Vec2f(1.0f, 1.0f);
    scalingEasingBaseScore_.time = scalingEasingBaseScore_.maxTime;
}

void ScoreStatus::TimeIncrementAnimation(const float& value) {
    if (!isChanging_) {
        return;
    }
    scoreAdaptEasing_.time += value;

    currentScore_ = EaseInOutQuint(preScore_, scoreUPValue_, scoreAdaptEasing_.time, scoreAdaptEasing_.maxTime);

    if (scoreAdaptEasing_.time < scoreAdaptEasing_.maxTime) {
        return;
    }

    currentScore_          = scoreUPValue_;
    scoreAdaptEasing_.time = scoreAdaptEasing_.maxTime;
    isChanging_            = false;
    preScore_              = currentScore_;
}

void ScoreStatus::SetTotalScoreValue() {
    scoreUPValue_ = totalScore_;
}

void ScoreStatus::ResetEaseTime() {

    scalingEasingBaseScore_.time = 0.0f;
    scaleEasing_.time            = 0.0f;
    scoreAdaptEasing_.time       = 0.0f;
    upScpreUpEasing_.time        = 0.0f;
}

void ScoreStatus::ResetPlusScore() {
    /* plusScore_       = 0.0f;*/
    pulusScoreValue_ = 0.0f;
}

void ScoreStatus::ScoreUPChange() {

    switch (scoreUIStep_) {

    case ScoreUIStep::WAIT:
        ResetEaseTime();
        scoreUIStep_ = ScoreUIStep::SCOREUP;
        break;
    default:
        scoreUIStep_ = ScoreUIStep::INIT;
        break;
    }
}

void to_json(nlohmann::json& _json, const ScoreStatus& _component) {
    _json["isAlive"]                   = _component.isAlive_;
    _json["currentScore"]              = _component.currentScore_;
    _json["pulusScore"]                = _component.pulusScoreValue_;
    _json["scoreAdaptEasing_.maxTime"] = _component.scoreAdaptEasing_.maxTime;
    _json["upScpreUpEasing_.maxTime"]  = _component.upScpreUpEasing_.maxTime;
    _json["scaleEasing_.maxTime"]      = _component.scaleEasing_.maxTime;
    _json["waitTime_"]                 = _component.waitTime_;

    _json["basePlusScoreTextureSize_"] = _component.basePlusScoreTextureSize_;
    _json["scaleEasing_.backRatio"]    = _component.scaleEasing_.backRatio;
    _json["scaleEasing_.amplitude"]    = _component.scaleEasing_.amplitude;
    _json["scaleEasing_.period"]       = _component.scaleEasing_.period;

    _json["scalingEasingBaseScore_.maxTime"]   = _component.scalingEasingBaseScore_.maxTime;
    _json["scalingEasingBaseScore_.backRatio"] = _component.scalingEasingBaseScore_.backRatio;
    _json["scalingEasingBaseScore_.amplitude"] = _component.scalingEasingBaseScore_.amplitude;
    _json["scalingEasingBaseScore_.period"]    = _component.scalingEasingBaseScore_.period;

    _json["baseScoreTextureSize_"] = _component.baseScoreTextureSize_;
}

void from_json(const nlohmann::json& _json, ScoreStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    _json.at("currentScore").get_to(_component.currentScore_);
    _json.at("pulusScore").get_to(_component.pulusScoreValue_);

    if (auto it = _json.find("scoreAdaptEasing_.maxTime"); it != _json.end()) {
        _json.at("scoreAdaptEasing_.maxTime").get_to(_component.scoreAdaptEasing_.maxTime);
    }

    if (auto it = _json.find("upScpreUpEasing_.maxTime"); it != _json.end()) {
        _json.at("upScpreUpEasing_.maxTime").get_to(_component.upScpreUpEasing_.maxTime);
    }

    if (auto it = _json.find("scaleEasing_.maxTime"); it != _json.end()) {
        _json.at("scaleEasing_.maxTime").get_to(_component.scaleEasing_.maxTime);
    }

    if (auto it = _json.find("waitTime_"); it != _json.end()) {
        _json.at("waitTime_").get_to(_component.waitTime_);
    }

    if (auto it = _json.find("basePlusScoreTextureSize_"); it != _json.end()) {
        _json.at("basePlusScoreTextureSize_").get_to(_component.basePlusScoreTextureSize_);
    }

    if (auto it = _json.find("scaleEasing_.backRatio"); it != _json.end()) {
        _json.at("scaleEasing_.backRatio").get_to(_component.scaleEasing_.backRatio);
    }

    if (auto it = _json.find("scaleEasing_.amplitude"); it != _json.end()) {
        _json.at("scaleEasing_.amplitude").get_to(_component.scaleEasing_.amplitude);
    }

    if (auto it = _json.find("scaleEasing_.period"); it != _json.end()) {
        _json.at("scaleEasing_.period").get_to(_component.scaleEasing_.period);
    }

    if (auto it = _json.find("scalingEasingBaseScore_.maxTime"); it != _json.end()) {
        _json.at("scalingEasingBaseScore_.maxTime").get_to(_component.scalingEasingBaseScore_.maxTime);
    }

    if (auto it = _json.find("scalingEasingBaseScore_.backRatio"); it != _json.end()) {
        _json.at("scalingEasingBaseScore_.backRatio").get_to(_component.scalingEasingBaseScore_.backRatio);
    }

    if (auto it = _json.find("scalingEasingBaseScore_.amplitude"); it != _json.end()) {
        _json.at("scalingEasingBaseScore_.amplitude").get_to(_component.scalingEasingBaseScore_.amplitude);
    }

    if (auto it = _json.find("scalingEasingBaseScore_.period"); it != _json.end()) {
        _json.at("scalingEasingBaseScore_.period").get_to(_component.scalingEasingBaseScore_.period);
    }

    if (auto it = _json.find("baseScoreTextureSize_"); it != _json.end()) {
        _json.at("baseScoreTextureSize_").get_to(_component.baseScoreTextureSize_);
    }
}
