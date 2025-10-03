#include "Button.h"

/// engine
// ecs

#include "input/Input.h"

/// externals
// imgui
#ifdef _DEBUG
#include "imgui/imgui.h"
#include "myGui/MyGui.h"
#endif // _DEBUG

void to_json(nlohmann::json& j, const Button& r) {
    /// ============ color ============ ///
    nlohmann::json normalColorJson  = nlohmann::json::object();
    nlohmann::json hoverColorJson   = nlohmann::json::object();
    nlohmann::json pressColorJson   = nlohmann::json::object();
    nlohmann::json releaseColorJson = nlohmann::json::object();

    to_json<4, float>(normalColorJson, r.normalColor_);
    to_json<4, float>(hoverColorJson, r.hoverColor_);
    to_json<4, float>(pressColorJson, r.pressColor_);
    to_json<4, float>(releaseColorJson, r.releaseColor_);

    j["normalColor"]  = normalColorJson;
    j["hoverColor"]   = hoverColorJson;
    j["pressColor"]   = pressColorJson;
    j["releaseColor"] = releaseColorJson;

    /// ============ shortcut key ============ ///
    nlohmann::json shortKeysJson = nlohmann::json::array();
    for (auto& key : r.shortcutKey_) {
        shortKeysJson.push_back(key);
    }
    j["shortKeys"] = shortKeysJson;

    /// ============ shortcut pad button ============ ///
    nlohmann::json shortcutPadButton = nlohmann::json::array();
    for (auto& button : r.shortcutPadButton_) {
        shortcutPadButton.push_back(button);
    }
    j["shortcutPadButton"] = shortcutPadButton;
}
void from_json(const nlohmann::json& j, Button& r) {
    /// ============ color ============ ///
    if (j.contains("normalColor")) {
        from_json<4, float>(j.at("normalColor"), r.normalColor_);
    }
    if (j.contains("hoverColor")) {
        from_json<4, float>(j.at("hoverColor"), r.hoverColor_);
    }
    if (j.contains("pressColor")) {
        from_json<4, float>(j.at("pressColor"), r.pressColor_);
    }
    if (j.contains("releaseColor")) {
        from_json<4, float>(j.at("releaseColor"), r.releaseColor_);
    }

    /// ============ shortcut key ============ ///
    if (j.contains("shortKeys") && j.at("shortKeys").is_array()) {
        r.shortcutKey_.clear();
        for (const auto& key : j.at("shortKeys")) {
            r.shortcutKey_.push_back(key.get<Key>());
        }
    }

    /// ============ shortcut pad button ============ ///
    if (j.contains("shortcutPadButton") && j.at("shortcutPadButton").is_array()) {
        r.shortcutPadButton_.clear();
        for (const auto& button : j.at("shortcutPadButton")) {
            r.shortcutPadButton_.push_back(button.get<PadButton>());
        }
    }
}

Button::Button() {
}

Button::~Button() {
}

void Button::Initialize(GameEntity* /*_entity*/) {}

void Button::Edit(Scene* /*_scene*/, GameEntity* /*_entity*/, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG
    std::string label = "Button Colors" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        ColorEditGuiCommand("Normal Color" + _parentLabel, normalColor_);
        ColorEditGuiCommand("Hover Color" + _parentLabel, hoverColor_);
        ColorEditGuiCommand("Press Color" + _parentLabel, pressColor_);
        ColorEditGuiCommand("Release Color" + _parentLabel, releaseColor_);

        ImGui::TreePop();
    }

    // key
    label = "Shortcut Key" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        // key
        ImGui::Text("Key");
        std::string comboLabel = "";
        for (int i = 0; i < shortcutKey_.size(); ++i) {
            ImGui::PushID(i);
            ImGui::Text("Key%d :: ", i);
            ImGui::SameLine();
            comboLabel = "##Key" + std::to_string(i) + _parentLabel;
            if (ImGui::BeginCombo(comboLabel.c_str(), keyNameMap.find(shortcutKey_[i])->second.c_str())) {
                for (auto& key : keyNameMap) {
                    bool isSelected = (shortcutKey_[i] == key.first);
                    if (ImGui::Selectable(key.second.c_str(), isSelected)) {
                        auto command = std::make_unique<SetterCommand<Key>>(&shortcutKey_[i], key.first);
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
        label = "Add Key" + _parentLabel;
        if (ImGui::Button(label.c_str())) {
            auto command = std::make_unique<AddElementCommand<std::vector<Key>>>(&shortcutKey_, Key::ESCAPE);
            EditorController::getInstance()->pushCommand(std::move(command));
           
        }
        ImGui::SameLine();
        // remove
        label = "Remove Key" + _parentLabel;
        if (ImGui::Button(label.c_str())) {
            if (shortcutKey_.size() > 0) {
                auto command = std::make_unique<EraseElementCommand<std::vector<Key>>>(&shortcutKey_, shortcutKey_.end() - 1);
                EditorController::getInstance()->pushCommand(std::move(command));
            }
        }

        ImGui::TreePop();
    }

    // pad button
    label = "Shortcut PadButton" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        // pad button
        ImGui::Text("PadButton");
        std::string comboLabel = "";
        for (int i = 0; i < shortcutPadButton_.size(); ++i) {
            ImGui::PushID(i);
            ImGui::Text("PadButton%d :: ", i);
            ImGui::SameLine();
            comboLabel = "##PadButton" + std::to_string(i) + _parentLabel;
            if (ImGui::BeginCombo(comboLabel.c_str(), padButtonNameMap.find(shortcutPadButton_[i])->second.c_str())) {
                for (auto& button : padButtonNameMap) {
                    bool isSelected = (shortcutPadButton_[i] == button.first);
                    if (ImGui::Selectable(button.second.c_str(), isSelected)) {
                        auto command = std::make_unique<SetterCommand<PadButton>>(&shortcutPadButton_[i], button.first);
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
        label = "Add PadButton" + _parentLabel;
        if (ImGui::Button(label.c_str())) {
            auto command = std::make_unique<AddElementCommand<std::vector<PadButton>>>(&shortcutPadButton_, PadButton::UP);
            EditorController::getInstance()->pushCommand(std::move(command));
           
        }
        ImGui::SameLine();
        // remove
        label = "Remove PadButton" + _parentLabel;
        if (ImGui::Button(label.c_str())) {
            if (shortcutPadButton_.size() > 0) {
                auto command = std::make_unique<EraseElementCommand<std::vector<PadButton>>>(&shortcutPadButton_, shortcutPadButton_.end() - 1);
                EditorController::getInstance()->pushCommand(std::move(command));
               
            }
        }

        ImGui::TreePop();
    }

#endif // _DEBUG
}

void Button::Finalize() {}
