#include "ComboStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"



void ComboStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
   
   
}

bool ComboStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

      isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    //isChange |= ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f);
  
    return isChange;

}

void ComboStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
   
}

void ComboStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
   
}

void ComboStatus::Finalize() {}

