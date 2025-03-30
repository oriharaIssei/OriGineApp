#include "PillerStates.h"

/// externals
#include "imgui/imgui.h"

void PillerStates::Initialize([[maybe_unused]] GameEntity* _entity) {}

bool PillerStates::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::InputInt("pillarHP", &pillarHP_);
    isChange |= ImGui::DragFloat("collisionSize", &collisionSize_,0.01f);

    return isChange;
}

void PillerStates::Save(BinaryWriter& _writer) {
    _writer.Write(isAlive_);
    _writer.Write(pillarHP_);
    _writer.Write(collisionSize_);
}

void PillerStates::Load(BinaryReader& _reader) {
    _reader.Read(isAlive_);
    _reader.Read(pillarHP_);
    _reader.Read(collisionSize_);
}

void PillerStates::Finalize() {}
