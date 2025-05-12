#include "SceneChanger.h"

/// engine
#define ENGINE_SCENE
#define RESOURCE_DIRECTORY
#include "EngineInclude.h"
#include "module/editor/EditorGroup.h"
#include "module/editor/IEditor.h"

/// lib
#include "myFileSystem/MyFileSystem.h"

/// externals
// imgui
#ifdef _DEBUG
#include "imgui/imgui.h"
#endif // _DEBUG

SceneChanger::SceneChanger() {
}

SceneChanger::~SceneChanger() {
}

void SceneChanger::Initialize(GameEntity* /* _entity*/) {}

bool SceneChanger::Edit() {
    bool changed = false;
    ImGui::Text("Next Scene Name :");
    ImGui::SameLine();
    if (ImGui::BeginCombo("##NextSceneName", nextSceneName_.c_str())) {
        for (const auto& [directory, sceneName] : myfs::searchFile(kApplicationResourceDirectory + "/scene/", "json")) {
            bool isSelected = nextSceneName_ == sceneName;
            if (ImGui::Selectable(sceneName.c_str(), isSelected)) {

                EditorGroup::getInstance()->pushCommand(
                    std::make_unique<SetterCommand<std::string>>(&nextSceneName_, sceneName));

                changed = true;
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
