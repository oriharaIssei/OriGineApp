#include "BottomFloorStates.h"

/// externals
#include "imgui/imgui.h"

void BottomFloorStates::Initialize([[maybe_unused]] GameEntity* _entity) {
    fieldRadius_ = 30.0f;
   /* settingAngle_ = 60.0f;*/
    floorNum_     = 6;
}

bool BottomFloorStates::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("FieldRadius", &fieldRadius_, 0.01f);
    isChange |= ImGui::SliderAngle("settingAngle", &settingAngle_);
    isChange |= ImGui::InputInt("floorNum", &floorNum_);
  
    return isChange;
}

void BottomFloorStates::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("fieldRadius", fieldRadius_);
    _writer.Write("settingAngle", settingAngle_);
    _writer.Write("floorNum", floorNum_);
   
}

void BottomFloorStates::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("fieldRadius", fieldRadius_);
    _reader.Read("settingAngle", settingAngle_);
    _reader.Read("floorNum", floorNum_);
}

void BottomFloorStates::Finalize() {}
