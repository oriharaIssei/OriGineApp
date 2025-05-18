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

    ImGui::Text("scoreAdaptEasing");
    isChange |= DragGuiCommand("scoreAdaptEasing_.maxTime", scoreAdaptEasing_.maxTime);

#endif // _DEBUG

    return isChange;
}

void ScoreStatus::Finalize() {}

void ScoreStatus::PlusScoreIncrement(const float& value) {
    pulusScoreValue_ += value;
}

void ScoreStatus::TimeIncrementAnimation(const float& value) {
    scoreAdaptEasing_.time += value;
    currentScore_ = EaseOutQuad(currentScore_,  pulusScoreValue_, scoreAdaptEasing_.time, scoreAdaptEasing_.maxTime);

    if (scoreAdaptEasing_.time < scoreAdaptEasing_.maxTime) {
        return;
    }
       
    scoreUIStep_           = ScoreUIStep::NONE;
    scoreAdaptEasing_.time = scoreAdaptEasing_.maxTime;
}

void ScoreStatus::Reset() {
    /*  isChanging_            = true;*/
    scoreAdaptEasing_.time = 0.0f;
    isChanging_            = false;
}

void to_json(nlohmann::json& _json, const ScoreStatus& _component) {
    _json["isAlive"]                   = _component.isAlive_;
    _json["currentScore"]              = _component.currentScore_;
    _json["pulusScore"]                = _component.pulusScoreValue_;
    _json["scoreAdaptEasing_.maxTime"] = _component.scoreAdaptEasing_.maxTime;
}

void from_json(const nlohmann::json& _json, ScoreStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    _json.at("currentScore").get_to(_component.currentScore_);
    _json.at("pulusScore").get_to(_component.pulusScoreValue_);

    if (auto it = _json.find("scoreAdaptEasing_.maxTime"); it != _json.end()) {
        _json.at("scoreAdaptEasing_.maxTime").get_to(_component.scoreAdaptEasing_.maxTime);
    }
}
