#include "FloorAndPillerrStatus.h"
/// externals
#include "imgui/imgui.h"
#include <string>

void FloorAndPillerrStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    fallPosY_ = 0.0f;
    fallEaseT_ = 0.0f;
    fallspeed_ = 3.0f;
}

bool FloorAndPillerrStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("fallspeed", &fallspeed_,0.01f);
   /* isChange |= ImGui::DragInt("row:%d",&rowNum_);
    isChange |= ImGui::DragInt("colum:%d", &columNum_);*/
   
    return isChange;
}

void FloorAndPillerrStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("fallspeed", fallspeed_);
}

void FloorAndPillerrStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("fallspeed", fallspeed_);
}

void FloorAndPillerrStatus::Finalize() {}

void FloorAndPillerrStatus::SetColumAndRow(const int32_t& colum, const int32_t& row) {
    rowNum_   = row;
    columNum_ = colum;
}

void FloorAndPillerrStatus::SetColumDecrement() {
    columNum_--;
}

float FloorAndPillerrStatus::GetFallPosY()  {
    fallPosY_ = savePosY_ - fallValue_;
    return fallPosY_;
}
