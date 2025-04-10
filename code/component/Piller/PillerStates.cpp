#include "PillerStates.h"

/// externals
#include "imgui/imgui.h"

void PillerStates::Initialize([[maybe_unused]] GameEntity* _entity) {
    currentHP_     = 2;
    collisionSize_ = 5.0f;
    HPMax_         = 2;
   
}

bool PillerStates::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::InputInt("pillarHP", &currentHP_);
    isChange |= ImGui::DragFloat("collisionSize", &collisionSize_, 0.01f);
   
    return isChange;
}

void PillerStates::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("HPMax", currentHP_);
    _writer.Write("collisionSize", collisionSize_);
}

void PillerStates::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("HPMax", currentHP_);
    _reader.Read("collisionSize", collisionSize_);
}

void PillerStates::Finalize() {}

void PillerStates::TakeDamage() {
    currentHP_--;
}

