#include "SceneChangerStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void SceneChangerStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool SceneChangerStatus::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    return isChange;
}

void SceneChangerStatus::Finalize() {

}

void to_json(nlohmann::json& j, const SceneChangerStatus& g) {
    j["isAlive"] = g.isAlive_;
}

void from_json(const nlohmann::json& j, SceneChangerStatus& g) {
    j.at("isAlive").get_to(g.isAlive_);
}
