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
    currentComboNum_ = 0;
    maxCombvoNum_    = 0;
    isUpdateCombo_   = false;
    currentTime_      = 0.0f;
}

bool ComboStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

   
    isChange |= ImGui::DragFloat("currentTime", &currentTime_, 0.01f);
  
    return isChange;

}

void ComboStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("currentComboNum", currentComboNum_);
    _writer.Write("maxCombvoNum", maxCombvoNum_);
    _writer.Write("continuationTime", continuationTime_);
    _writer.Write("currentTime", currentTime_);
   
}

void ComboStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("maxCombvoNum", maxCombvoNum_);
    _reader.Read("isUpdateCombo", isUpdateCombo_);
    _reader.Read("continuationTime_", continuationTime_);
    _reader.Read("currentTime", currentTime_);
   
}

void ComboStatus::Finalize() {}

