#include "PillerStates.h"

/// externals
#include "imgui/imgui.h"

void PillerStates::Initialize([[maybe_unused]] GameEntity* _entity) {
   /* pillarHP_ = 0;
    collisionSize_ = 0.0f;*/
    currentHP_ = 0;
}

bool PillerStates::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::InputInt("pillarHP", &HPMax_);
    isChange |= ImGui::DragFloat("collisionSize", &collisionSize_,0.01f);

    return isChange;
}

void PillerStates::Save(BinaryWriter& _writer) {
    _writer.Write(isAlive_);
    _writer.Write(HPMax_);
    _writer.Write(collisionSize_);
}

void PillerStates::Load(BinaryReader& _reader) {
    _reader.Read(isAlive_);
    _reader.Read(HPMax_);
    _reader.Read(collisionSize_);
}

void PillerStates::Finalize() {}

void PillerStates::TakeDamage() {
    currentHP_--;
 }
