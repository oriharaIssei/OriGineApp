#include "ButtonGroup.h"

/// engine
#include "scene/Scene.h"

/// ecs
#include "Button.h"

/// util
#include "editor/EditorController.h"
#include "myGui/MyGui.h"

void ButtonGroup::Initialize(Entity* /*_entity*/) {}

void ButtonGroup::Edit([[maybe_unused]] Scene* _scene, [[maybe_unused]] Entity* _entity, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG
    auto componentArray = _scene->getComponentRepositoryRef()->getComponentArray<Button>();
    if (!componentArray) {
        ImGui::Text(" No ButtonData.");
        return;
    }

    auto allButtons = componentArray->getAllComponents();
    if (allButtons->empty()) {
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
        const auto& buttonBindEntityId = componentArray->getEntityIndexBind();
        for (const auto& [id, index] : buttonBindEntityId) {
            if (id < 0) {
                continue;
            }
            if (buttonNumbers_.find(id) == buttonNumbers_.end()) {
                buttonNumbers_[id] = int32_t(buttonNumbers_.size());
            }

            ImGui::Text("EntityID [%d] :", id);
            ImGui::SameLine();
            ImGui::Text("Button No. %d ", buttonNumbers_[id]);

            ImGui::Spacing();
        }
        ImGui::TreePop();
    }

    ImGui::Separator();

    auto editKeyBind = [this](const std::string& _editParentLabel, std::vector<Key>& _keyBinds) {
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
                        auto command = std::make_unique<SetterCommand<Key>>(&_keyBinds[i], key.first);
                        EditorController::getInstance()->pushCommand(std::move(command));
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
            auto command = std::make_unique<AddElementCommand<std::vector<Key>>>(&_keyBinds, Key::ESCAPE);
            EditorController::getInstance()->pushCommand(std::move(command));
        }
        ImGui::SameLine();
        // remove
        editorLabel = "Remove Key##" + _editParentLabel;
        if (ImGui::Button(editorLabel.c_str())) {
            if (_keyBinds.size() > 0) {
                auto command = std::make_unique<EraseElementCommand<std::vector<Key>>>(&_keyBinds, _keyBinds.end() - 1);
                EditorController::getInstance()->pushCommand(std::move(command));
            }
        }
    };
    auto editPadButtonBind = [this](const std::string& _editParentLabel, std::vector<PadButton>& _padButtonBinds) {
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
                        auto command = std::make_unique<SetterCommand<PadButton>>(&_padButtonBinds[i], padButton.first);
                        EditorController::getInstance()->pushCommand(std::move(command));
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
            auto command = std::make_unique<AddElementCommand<std::vector<PadButton>>>(&_padButtonBinds, PadButton::A);
            EditorController::getInstance()->pushCommand(std::move(command));
        }
        ImGui::SameLine();
        // remove
        editorLabel = "Remove PadButton##" + _editParentLabel;
        if (ImGui::Button(editorLabel.c_str())) {
            if (_padButtonBinds.size() > 0) {
                auto command = std::make_unique<EraseElementCommand<std::vector<PadButton>>>(&_padButtonBinds, _padButtonBinds.end() - 1);
                EditorController::getInstance()->pushCommand(std::move(command));
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
        buttonNumberJson["entityID"]     = buttonNumber.first;
        buttonNumberJson["buttonNumber"] = buttonNumber.second;
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
    if (j.contains("buttonNumbers") && j.at("buttonNumbers").is_array()) {
        r.buttonNumbers_.clear();
        for (const auto& buttonNumberJson : j.at("buttonNumbers")) {
            if (buttonNumberJson.contains("entityID") && buttonNumberJson.contains("buttonNumber")) {
                int32_t entityID           = buttonNumberJson.at("entityID").get<int32_t>();
                int32_t buttonNumber       = buttonNumberJson.at("buttonNumber").get<int32_t>();
                r.buttonNumbers_[entityID] = buttonNumber;
            }
        }
    }
    if (j.contains("selectAddKeys") && j.at("selectAddKeys").is_array()) {
        r.selectAddKeys_.clear();
        for (const auto& key : j.at("selectAddKeys")) {
            r.selectAddKeys_.push_back(key.get<Key>());
        }
    }
    if (j.contains("selectAddPadButtons") && j.at("selectAddPadButtons").is_array()) {
        r.selectAddPadButtons_.clear();
        for (const auto& button : j.at("selectAddPadButtons")) {
            r.selectAddPadButtons_.push_back(button.get<PadButton>());
        }
    }
    if (j.contains("selectSubKeys") && j.at("selectSubKeys").is_array()) {
        r.selectSubKeys_.clear();
        for (const auto& key : j.at("selectSubKeys")) {
            r.selectSubKeys_.push_back(key.get<Key>());
        }
    }
    if (j.contains("selectSubPadButtons") && j.at("selectSubPadButtons").is_array()) {
        r.selectSubPadButtons_.clear();
        for (const auto& button : j.at("selectSubPadButtons")) {
            r.selectSubPadButtons_.push_back(button.get<PadButton>());
        }
    }
    if (j.contains("decideKeys") && j.at("decideKeys").is_array()) {
        r.decideKeys_.clear();
        for (const auto& key : j.at("decideKeys")) {
            r.decideKeys_.push_back(key.get<Key>());
        }
    }
    if (j.contains("decidePadButtons") && j.at("decidePadButtons").is_array()) {
        r.decidePadButtons_.clear();
        for (const auto& button : j.at("decidePadButtons")) {
            r.decidePadButtons_.push_back(button.get<PadButton>());
        }
    }
}
