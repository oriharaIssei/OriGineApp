#include "ButtonGroup.h"

/// engine
#include "scene/Scene.h"

/// ecs
#include "Button.h"

/// util
#include "editor/EditorController.h"
#include "myGui/MyGui.h"

using namespace OriGine;

void ButtonGroup::Initialize(Scene* /*_scene*/, EntityHandle /*_owner*/) {}

void ButtonGroup::Edit([[maybe_unused]] OriGine::Scene* _scene, [[maybe_unused]] EntityHandle _owner, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG
    auto componentArray = _scene->GetComponentRepositoryRef()->GetComponentArray<Button>();
    if (!componentArray) {
        ImGui::Text(" No ButtonData.");
        return;
    }

    auto& allButtons = componentArray->GetSlots();
    if (allButtons.empty()) {
        if (!buttonNumbers_.empty()) {
            buttonNumbers_.clear();
        }
        ImGui::Text(" No ButtonData.");
        return;
    }

    InputGuiCommand("StartButtonIndex##" + _parentLabel, startButtonNumber_, "%2d");
    if (buttonNumbers_.empty()) {
        startButtonNumber_ = 0;
    } else {
        startButtonNumber_   = std::clamp(startButtonNumber_, 0, static_cast<int32_t>(buttonNumbers_.size()) - 1);
        currentButtonNumber_ = startButtonNumber_;
    }

    std::string label = "ButtonNumbers##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        buttonNumbers_.clear();
        int32_t buttonIndex = 0;
        for (const auto& slot : allButtons) {
            // ボタンコンポーネントが存在しない場合はスキップ
            OriGine::Entity* buttonEntity = _scene->GetEntityRepositoryRef()->GetEntity(slot.owner);
            if (!buttonEntity || componentArray->HasEntity(slot.owner)) {
                continue;
            }

            buttonNumbers_[buttonIndex] = slot.owner;

            ImGui::Text("EntityID [%s] :", uuids::to_string(slot.owner.uuid).c_str());
            ImGui::SameLine();
            ImGui::Text("Button No. %d ", buttonIndex);
            ++buttonIndex;

            ImGui::Spacing();
        }
        ImGui::TreePop();
    }

    ImGui::Separator();

    auto editKeyBind = [this](const std::string& _editParentLabel, std::vector<OriGine::Key>& _keyBinds) {
        std::string editorLabel = "";

        for (int i = 0; i < _keyBinds.size(); ++i) {
            ImGui::PushID(i);
            ImGui::Text("Key%d :: ", i);
            ImGui::SameLine();
            editorLabel = "##Key" + std::to_string(i) + _editParentLabel;
            if (ImGui::BeginCombo(editorLabel.c_str(), keyNameMap.find(_keyBinds[i])->second.c_str())) {
                for (auto& key : keyNameMap) {
                    bool isSelected = (_keyBinds[i] == key.first);
                    if (ImGui::Selectable(key.second.c_str(), isSelected)) {
                        auto command = std::make_unique<SetterCommand<OriGine::Key>>(&_keyBinds[i], key.first);
                        OriGine::EditorController::GetInstance()->PushCommand(std::move(command));
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::PopID();
        }
        // add
        editorLabel = "Add Key##" + _editParentLabel;
        if (ImGui::Button(editorLabel.c_str())) {
            auto command = std::make_unique<AddElementCommand<std::vector<OriGine::Key>>>(&_keyBinds, OriGine::Key::ESCAPE);
            OriGine::EditorController::GetInstance()->PushCommand(std::move(command));
        }
        ImGui::SameLine();
        // remove
        editorLabel = "Remove Key##" + _editParentLabel;
        if (ImGui::Button(editorLabel.c_str())) {
            if (_keyBinds.size() > 0) {
                auto command = std::make_unique<EraseElementCommand<std::vector<OriGine::Key>>>(&_keyBinds, _keyBinds.end() - 1);
                OriGine::EditorController::GetInstance()->PushCommand(std::move(command));
            }
        }
    };
    auto editPadButtonBind = [this](const std::string& _editParentLabel, std::vector<OriGine::PadButton>& _padButtonBinds) {
        std::string editorLabel = "";
        for (int i = 0; i < _padButtonBinds.size(); ++i) {
            ImGui::PushID(i);
            ImGui::Text("PadButton%d :: ", i);
            ImGui::SameLine();
            editorLabel = "##PadButton" + std::to_string(i) + _editParentLabel;
            if (ImGui::BeginCombo(editorLabel.c_str(), padButtonNameMap.find(_padButtonBinds[i])->second.c_str())) {
                for (auto& padButton : padButtonNameMap) {
                    bool isSelected = (_padButtonBinds[i] == padButton.first);
                    if (ImGui::Selectable(padButton.second.c_str(), isSelected)) {
                        auto command = std::make_unique<SetterCommand<OriGine::PadButton>>(&_padButtonBinds[i], padButton.first);
                        OriGine::EditorController::GetInstance()->PushCommand(std::move(command));
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::PopID();
        }
        // add
        editorLabel = "Add PadButton##" + _editParentLabel;
        if (ImGui::Button(editorLabel.c_str())) {
            auto command = std::make_unique<AddElementCommand<std::vector<OriGine::PadButton>>>(&_padButtonBinds, OriGine::PadButton::A);
            OriGine::EditorController::GetInstance()->PushCommand(std::move(command));
        }
        ImGui::SameLine();
        // remove
        editorLabel = "Remove PadButton##" + _editParentLabel;
        if (ImGui::Button(editorLabel.c_str())) {
            if (_padButtonBinds.size() > 0) {
                auto command = std::make_unique<EraseElementCommand<std::vector<OriGine::PadButton>>>(&_padButtonBinds, _padButtonBinds.end() - 1);
                OriGine::EditorController::GetInstance()->PushCommand(std::move(command));
            }
        }
    };

    label = "SelectAddKeyBind##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        editKeyBind("SelectAddKeyBind##" + _parentLabel, selectAddKeys_);
        ImGui::Separator();
        editPadButtonBind("SelectAddPadButtonBind##" + _parentLabel, selectAddPadButtons_);
        ImGui::TreePop();
    }
    label = "SelectSubKeyBind##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        editKeyBind("SelectSubKeyBind##" + _parentLabel, selectSubKeys_);
        ImGui::Separator();
        editPadButtonBind("SelectSubPadButtonBind##" + _parentLabel, selectSubPadButtons_);
        ImGui::TreePop();
    }
    label = "DecideKeyBind##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        editKeyBind("DecideKeyBind##" + _parentLabel, decideKeys_);
        ImGui::Separator();
        editPadButtonBind("DecidePadButtonBind##" + _parentLabel, decidePadButtons_);
        ImGui::TreePop();
    }

#endif // _DEBUG
}

void ButtonGroup::Finalize() {
}

void to_json(nlohmann::json& j, const ButtonGroup& r) {
    j["startButtonNumber"] = r.startButtonNumber_;

    nlohmann::json buttonNumbersJson = nlohmann::json::array();
    for (auto& buttonNumber : r.buttonNumbers_) {
        nlohmann::json buttonNumberJson;
        buttonNumberJson["buttonNumber"] = buttonNumber.first;
        buttonNumberJson["entityHandle"] = buttonNumber.second;
        buttonNumbersJson.push_back(buttonNumberJson);
    }
    j["buttonNumbers"] = buttonNumbersJson;

    j["selectAddKeys"]       = r.selectAddKeys_;
    j["selectAddPadButtons"] = r.selectAddPadButtons_;
    j["selectSubKeys"]       = r.selectSubKeys_;
    j["selectSubPadButtons"] = r.selectSubPadButtons_;
    j["decideKeys"]          = r.decideKeys_;
    j["decidePadButtons"]    = r.decidePadButtons_;
}

void from_json(const nlohmann::json& j, ButtonGroup& r) {
    if (j.contains("startButtonNumber")) {
        r.startButtonNumber_ = j.at("startButtonNumber").get<int32_t>();
    }
    /* if (j.contains("buttonNumbers") && j.at("buttonNumbers").is_array()) {
         r.buttonNumbers_.clear();
         for (const auto& buttonNumberJson : j.at("buttonNumbers")) {
             if (buttonNumberJson.contains("entityID") && buttonNumberJson.contains("buttonNumber")) {
                 EntityHandle entityID      = buttonNumberJson.at("entityHandle").get<EntityHandle>();
                 int32_t buttonNumber       = buttonNumberJson.at("buttonNumber").get<int32_t>();
                 r.buttonNumbers_[buttonNumber] = entityID;
             }
         }
     }*/
    if (j.contains("selectAddKeys") && j.at("selectAddKeys").is_array()) {
        r.selectAddKeys_.clear();
        for (const auto& key : j.at("selectAddKeys")) {
            r.selectAddKeys_.push_back(key.get<OriGine::Key>());
        }
    }
    if (j.contains("selectAddPadButtons") && j.at("selectAddPadButtons").is_array()) {
        r.selectAddPadButtons_.clear();
        for (const auto& button : j.at("selectAddPadButtons")) {
            r.selectAddPadButtons_.push_back(button.get<OriGine::PadButton>());
        }
    }
    if (j.contains("selectSubKeys") && j.at("selectSubKeys").is_array()) {
        r.selectSubKeys_.clear();
        for (const auto& key : j.at("selectSubKeys")) {
            r.selectSubKeys_.push_back(key.get<OriGine::Key>());
        }
    }
    if (j.contains("selectSubPadButtons") && j.at("selectSubPadButtons").is_array()) {
        r.selectSubPadButtons_.clear();
        for (const auto& button : j.at("selectSubPadButtons")) {
            r.selectSubPadButtons_.push_back(button.get<OriGine::PadButton>());
        }
    }
    if (j.contains("decideKeys") && j.at("decideKeys").is_array()) {
        r.decideKeys_.clear();
        for (const auto& key : j.at("decideKeys")) {
            r.decideKeys_.push_back(key.get<OriGine::Key>());
        }
    }
    if (j.contains("decidePadButtons") && j.at("decidePadButtons").is_array()) {
        r.decidePadButtons_.clear();
        for (const auto& button : j.at("decidePadButtons")) {
            r.decidePadButtons_.push_back(button.get<OriGine::PadButton>());
        }
    }
}
