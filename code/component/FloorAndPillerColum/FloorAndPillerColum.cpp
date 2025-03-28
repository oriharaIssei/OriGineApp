#include "FloorAndPillerColum.h"
/// externals
#include "imgui/imgui.h"


void FloorAndPillerColum::Initialize([[maybe_unused]] GameEntity* _entity) {
   
}

bool FloorAndPillerColum::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("FieldRadius", &fallSpeed_, 0.01f);
    isChange |= ImGui::InputInt("columnHeight", &columnHeight_);
    isChange |= ImGui::InputInt("gridNum", &gridNum_);

    return isChange;
}

void FloorAndPillerColum::Save(BinaryWriter& _writer) {
    _writer.Write(fallSpeed_);
    _writer.Write(columnHeight_);
    _writer.Write(gridNum_);
    
}

void FloorAndPillerColum::Load(BinaryReader& _reader) {
    _reader.Read(fallSpeed_);
    _reader.Read(columnHeight_);
    _reader.Read(gridNum_);
 
}

void FloorAndPillerColum::Finalize() {}

