#include "FloatingFloorStatus.h"
/// externals
#include "imgui/imgui.h"
#include <string>

void FloatingFloorStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    fallPosY_ = 0.0f;
    fallEaseT_ = 0.0f;
    fallspeed_ = 3.0f;
    currentHP_     = 2;
   /* collisionSize_ = 5.0f;*/
}

bool FloatingFloorStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("fallspeed", &fallspeed_, 0.01f);
   
    return isChange;
}

void FloatingFloorStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("fallspeed", fallspeed_);
   
}

void FloatingFloorStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("fallspeed", fallspeed_);
   
}

void FloatingFloorStatus::Finalize() {}

void FloatingFloorStatus::SetColumAndRow(const int32_t& colum, const int32_t& row) {
    rowNum_   = row;
    columNum_ = colum;
}

void FloatingFloorStatus::SetColumDecrement() {
    columNum_--;
}

float FloatingFloorStatus::GetFallPosY()  {
    fallPosY_ = savePosY_ - fallValue_;
    return fallPosY_;
}

void FloatingFloorStatus::TakeDamage() {
    currentHP_--;
}
