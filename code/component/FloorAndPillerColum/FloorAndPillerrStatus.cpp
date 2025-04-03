#include "FloorAndPillerrStatus.h"
/// externals
#include "imgui/imgui.h"
#include<string>

void FloorAndPillerrStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
   
}

bool FloorAndPillerrStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

   
   
    
    return isChange;
}

void FloorAndPillerrStatus::Save(BinaryWriter& _writer) {
    _writer.Write(isAlive_);
  
}

void FloorAndPillerrStatus::Load(BinaryReader& _reader) {
    _reader.Read(isAlive_);
 
}

void FloorAndPillerrStatus::Finalize() {}

void FloorAndPillerrStatus::SetColumAndRow(const int32_t& colum, const int32_t& row) {
    rowNum_ = row;
    columNum_ = colum;
  }
