#include "CharacterStatus.h"

/// externals
#include "imgui/imgui.h"

void CharacterStatus::Initialize([[maybe_unused]]GameEntity* _entity) {}

bool CharacterStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::InputInt("Life", &life_);
    isChange |= ImGui::InputInt("Attack", &attack_);

    return isChange;
}

void CharacterStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("life", life_);
    _writer.Write("attack", attack_);
}

void CharacterStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("life", life_);
    _reader.Read("attack", attack_);
}

void CharacterStatus::Finalize() {}
