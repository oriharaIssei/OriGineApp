#include "ScoreStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

float ScoreStatus::currentScore_ = 0.0f;

void ScoreStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ScoreStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("currentScore", &currentScore_);
    isChange |= ImGui::DragFloat("pulusScore", &pulusScore_);

    return isChange;
}

void ScoreStatus::Finalize() {}

void ScoreStatus::PlusScoreIncrement(const float& value) {
    pulusScore_ += value;
}

void ScoreStatus::TimeIncrement(const float& value) {
    scoreChangeTime_ += value;
    if (scoreChangeTime_ >= 1.0f) {
        scoreChangeTime_ = 1.0f;
    }
}

void to_json(nlohmann::json& _json, const ScoreStatus& _component) {
    _json["isAlive"]      = _component.isAlive_;
    _json["currentScore"] = _component.currentScore_;
    _json["pulusScore"]   = _component.pulusScore_;
}

void from_json(const nlohmann::json& _json, ScoreStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    _json.at("currentScore").get_to(_component.currentScore_);
    _json.at("pulusScore").get_to(_component.pulusScore_);
}
