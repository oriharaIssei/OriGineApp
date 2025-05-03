#include "GameEnd.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void GameEnd::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool GameEnd::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    return isChange;
}

void GameEnd::Finalize() {

}

void to_json(nlohmann::json& j, const GameEnd& g) {
    j["isAlive"] = g.isAlive_;
}

void from_json(const nlohmann::json& j, GameEnd& g) {
    j.at("isAlive").get_to(g.isAlive_);
}
