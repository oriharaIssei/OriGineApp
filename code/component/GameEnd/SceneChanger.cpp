#include "SceneChanger.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void SceneChanger::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool SceneChanger::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    return isChange;
}

void SceneChanger::Finalize() {

}

void to_json(nlohmann::json& j, const SceneChanger& g) {
    j["isAlive"] = g.isAlive_;
}

void from_json(const nlohmann::json& j, SceneChanger& g) {
    j.at("isAlive").get_to(g.isAlive_);
}
