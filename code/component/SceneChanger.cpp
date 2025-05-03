#include "SceneChanger.h"

/// engine
#define ENGINE_SCENE
#include "EngineInclude.h"

/// externals
// imgui
#include "imgui/imgui.h"

SceneChanger::SceneChanger() {
}

SceneChanger::~SceneChanger() {
}

void SceneChanger::Initialize(GameEntity* /*_entity*/) {}

bool SceneChanger::Edit() {
    bool changed = false;
    ImGui::Text("Next Scene Name :");
    ImGui::SameLine();
    if (ImGui::BeginCombo("##NextSceneName", nextSceneName_.c_str())) {
        SceneManager* sceneManager = SceneManager::getInstance();
        for (const auto& [sceneName, sceneIndex] : sceneManager->getScenes()) {
            bool isSelected = nextSceneName_ == sceneName;
            if (ImGui::Selectable(sceneName.c_str(), isSelected)) {
                nextSceneName_ = sceneName;
                changed        = true;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    return changed;
}

void SceneChanger::Finalize() {}

void to_json(nlohmann::json& j, const SceneChanger& r) {
    j["nextSceneName"] = r.nextSceneName_;
}

void from_json(const nlohmann::json& j, SceneChanger& r) {
    j.at("nextSceneName").get_to(r.nextSceneName_);
}
