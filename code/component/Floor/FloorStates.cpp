#include "FloorStates.h"

/// externals
#include "imgui/imgui.h"

void FloorStates::Initialize([[maybe_unused]] GameEntity* _entity) {
  /*  fallTimer_ = 0.0f;*/
}

bool FloorStates::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("fallTimer", &fallTimer_, 0.01f);
  
    return isChange;
}

void FloorStates::Save(BinaryWriter& _writer) {
    _writer.Write(isAlive_);
    _writer.Write(fallTimer_);
}

void FloorStates::Load(BinaryReader& _reader) {
    _reader.Read(isAlive_);
    _reader.Read(fallTimer_);
}

void FloorStates::Finalize() {}
