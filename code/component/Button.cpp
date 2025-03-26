#include "Button.h"

/// engine
// ecs
#include "ECSManager.h"
#include "input/Input.h"

/// externals
// imgui
#include "imgui/imgui.h"

Button::Button() {
}

Button::~Button() {
}

void Button::Initialize(GameEntity* /*_entity*/) {}

bool Button::Edit() {
    bool isChanged = false;

    if (ImGui::TreeNode("Button Colors")) {
        isChanged |= ImGui::ColorEdit4("Normal Color", normalColor_.v);
        isChanged |= ImGui::ColorEdit4("Hover Color", hoverColor_.v);
        isChanged |= ImGui::ColorEdit4("Press Color", pressColor_.v);
        isChanged |= ImGui::ColorEdit4("Release Color", releaseColor_.v);

        ImGui::TreePop();
    }

    // key
    if (ImGui::TreeNode("Shortcut Key")) {
        // key
        ImGui::Text("Key");
        std::string comboLabel = "";
        for (int i = 0; i < shortcutKey_.size(); ++i) {
            ImGui::PushID(i);
            ImGui::Text("Key%d :: ", i);
            ImGui::SameLine();
            comboLabel = "##Key" + std::to_string(i);
            if (ImGui::BeginCombo(comboLabel.c_str(), keyNameMap.find(shortcutKey_[i])->second.c_str())) {
                for (auto& key : keyNameMap) {
                    bool isSelected = (shortcutKey_[i] == key.first);
                    if (ImGui::Selectable(key.second.c_str(), isSelected)) {
                        shortcutKey_[i] = key.first;
                        isChanged       = true;
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
        if (ImGui::Button("Add Key")) {
            shortcutKey_.push_back(Key::ESCAPE);
            isChanged = true;
        }
        ImGui::SameLine();
        // remove
        if (ImGui::Button("Remove Key")) {
            if (shortcutKey_.size() > 0) {
                shortcutKey_.pop_back();
                isChanged = true;
            }
        }

        ImGui::TreePop();
    }

    // pad button
    if (ImGui::TreeNode("Shortcut PadButton")) {
        // pad button
        ImGui::Text("PadButton");
        std::string comboLabel = "";
        for (int i = 0; i < shortcutPadButton_.size(); ++i) {
            ImGui::PushID(i);
            ImGui::Text("PadButton%d :: ", i);
            ImGui::SameLine();
            comboLabel = "##PadButton" + std::to_string(i);
            if (ImGui::BeginCombo(comboLabel.c_str(), padButtonNameMap.find(shortcutPadButton_[i])->second.c_str())) {
                for (auto& button : padButtonNameMap) {
                    bool isSelected = (shortcutPadButton_[i] == button.first);
                    if (ImGui::Selectable(button.second.c_str(), isSelected)) {
                        shortcutPadButton_[i] = button.first;
                        isChanged             = true;
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
        if (ImGui::Button("Add PadButton")) {
            shortcutPadButton_.push_back(PadButton::UP);
            isChanged = true;
        }
        ImGui::SameLine();
        // remove
        if (ImGui::Button("Remove PadButton")) {
            if (shortcutPadButton_.size() > 0) {
                shortcutPadButton_.pop_back();
                isChanged = true;
            }
        }

        ImGui::TreePop();
    }

    return isChanged;
}

void Button::Save(BinaryWriter& _writer) {
    /// ============ color ============ ///
    _writer.Write<4, float>(normalColor_);
    _writer.Write<4, float>(hoverColor_);
    _writer.Write<4, float>(pressColor_);
    _writer.Write<4, float>(releaseColor_);

    /// ============ shortcut key ============ ///
    // size
    _writer.Write<1, uint32_t>(shortcutKey_.size());
    // data
    for (auto& key : shortcutKey_) {
        _writer.Write<1, int32_t>(static_cast<int32_t>(key));
    }

    /// ============ shortcut pad button ============ ///
    // size
    _writer.Write<1, uint32_t>(shortcutPadButton_.size());
    // data
    for (auto& button : shortcutPadButton_) {
        _writer.Write<1, int32_t>(static_cast<int32_t>(button));
    }
}

void Button::Load(BinaryReader& _reader) {
    /// ============ color ============ ///
    _reader.Read<4, float>(normalColor_);
    _reader.Read<4, float>(hoverColor_);
    _reader.Read<4, float>(pressColor_);
    _reader.Read<4, float>(releaseColor_);

    /// ============ shortcut key ============ ///
    // size
    uint32_t size = 0;
    _reader.Read<uint32_t>(size);
    shortcutKey_.resize(size);

    // data
    int32_t dataInt = 0;
    for (auto& key : shortcutKey_) {
        _reader.Read<int32_t>(dataInt);
        key = static_cast<Key>(dataInt);
    }

    /// ============ shortcut pad button ============ ///
    // size
    _reader.Read<uint32_t>(size);
    shortcutPadButton_.resize(size);

    // data
    for (auto& button : shortcutPadButton_) {
        _reader.Read<int32_t>(dataInt);
        button = static_cast<PadButton>(dataInt);
    }
}

void Button::Finalize() {}
