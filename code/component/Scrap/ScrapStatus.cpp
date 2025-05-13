#include "ScrapStatus.h"

/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void ScrapStatus::Initialize([[maybe_unused]] GameEntity* _entity) {}

bool ScrapStatus::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    return isChange;
}

void ScrapStatus::Finalize() {}

void ScrapStatus::LifeTimeDecrement(const float& decrementValue) {
    lifeTime_ -= decrementValue;
}

void to_json(nlohmann::json& _json, const ScrapStatus& _component) {
    _json["isAlive"] = _component.isAlive_;
}

void from_json(const nlohmann::json& _json, ScrapStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
}
