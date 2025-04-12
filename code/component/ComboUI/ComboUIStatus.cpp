#include "ComboUIStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"



void ComboUIStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
   
   
}

bool ComboUIStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

      isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    //isChange |= ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f);
  
    return isChange;

}

void ComboUIStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
   
}

void ComboUIStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
   
}

void ComboUIStatus::Finalize() {}

int32_t ComboUIStatus::GetComboDidit(const ComboDigit& _ditit) {
    return comboDigit_[static_cast<size_t>(_ditit)];
 }
