#include "FieldStates.h"

/// externals
#include "imgui/imgui.h"

void FieldStates::Initialize([[maybe_unused]] GameEntity* _entity) {}

bool FieldStates::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("MoveSpeed", &fieldRadius_, 0.01f);
  
    return isChange;

}

void FieldStates::Save(BinaryWriter& _writer) {
    _writer.Write(isAlive_);
    _writer.Write(fieldRadius_);
}

void FieldStates::Load(BinaryReader& _reader) {
    _reader.Read(isAlive_);
    _reader.Read(fieldRadius_);
}

void FieldStates::Finalize() {}
