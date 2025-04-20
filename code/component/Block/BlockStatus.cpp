#include "BlockStatus.h"
/// externals
#include "imgui/imgui.h"
#include <string>

void BlockStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
   
    currentHP_     = 1;
   /* collisionSize_ = 5.0f;*/
}

bool BlockStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();
   
    return isChange;
}

void BlockStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
   
}

void BlockStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
   
}

void BlockStatus::Finalize() {}

void BlockStatus::SetColum(const int32_t& colum) {
   
    columNum_ = colum;
}

void BlockStatus::TakeDamage() {
    currentHP_--;
}
