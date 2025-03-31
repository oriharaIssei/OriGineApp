#include "FloorAndPillerSpawner.h"
/// externals
#include "imgui/imgui.h"
#include<string>

void FloorAndPillerSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {
    floorSpace_ = 11.0f;
    pillerSpace_ = 10.0f;
    columNum_    = 5;
    firstPillerOffset_ = 5.5f;
    isCreated_=false;
}

bool FloorAndPillerSpawner::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

   
    isChange |= ImGui::DragFloat("floorSpace", &floorSpace_, 0.01f);
    isChange |= ImGui::DragFloat("pillerSpace", &pillerSpace_, 0.01f);
    isChange |= ImGui::DragFloat("firstPillerOffset", &firstPillerOffset_, 0.01f);
    isChange |= ImGui::InputInt("ColumNum", &columNum_);
    
    return isChange;
}

void FloorAndPillerSpawner::Save(BinaryWriter& _writer) {
    _writer.Write(pillerSpace_);
    _writer.Write(floorSpace_);
    _writer.Write(columNum_);
  
}

void FloorAndPillerSpawner::Load(BinaryReader& _reader) {
    _reader.Read(pillerSpace_);
    _reader.Read(floorSpace_);
    _reader.Read(columNum_);
 
}

void FloorAndPillerSpawner::Finalize() {}

