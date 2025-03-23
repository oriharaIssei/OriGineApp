#include "PlayerStates.h"

/// externals
#include "imgui/imgui.h"

void PlayerStates::Initialize([[maybe_unused]] GameEntity* _entity) {}

bool PlayerStates::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f);
    isChange |= ImGui::InputInt("SettingBomNum_", &settingBomNum_);

    return isChange;
}

void PlayerStates::Save(BinaryWriter& _writer) {
    _writer.Write(isAlive_);
    _writer.Write(moveSpeed_);
    _writer.Write(settingBomNum_);
}

void PlayerStates::Load(BinaryReader& _reader) {
    _reader.Read(isAlive_);
    _reader.Read(moveSpeed_);
    _reader.Read(settingBomNum_);
}

void PlayerStates::Finalize() {}
