#include "BottomFloorStates.h"

/// externals
#include "imgui/imgui.h"

void BottomFloorStates::Initialize([[maybe_unused]] GameEntity* _entity) {}

bool BottomFloorStates::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("FieldRadius", &fieldRadius_, 0.01f);
   
  
    return isChange;
}

void BottomFloorStates::Save(BinaryWriter& _writer) {
    _writer.Write(isAlive_);
    _writer.Write(fieldRadius_);
   
}

void BottomFloorStates::Load(BinaryReader& _reader) {
    _reader.Read(isAlive_);
    _reader.Read(fieldRadius_);
    
}

void BottomFloorStates::Finalize() {}
