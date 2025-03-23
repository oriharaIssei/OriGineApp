#include "PillerStates.h"

/// externals
#include "imgui/imgui.h"

void PillerStates::Initialize([[maybe_unused]] GameEntity* _entity) {}

bool PillerStates::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::InputInt("pillarHP", &pillarHP_);

    return isChange;
}

void PillerStates::Save(BinaryWriter& _writer) {
    _writer.Write(isAlive_);
    _writer.Write(pillarHP_);
}

void PillerStates::Load(BinaryReader& _reader) {
    _reader.Read(isAlive_);
    _reader.Read(pillarHP_);
}

void PillerStates::Finalize() {}
