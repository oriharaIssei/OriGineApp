#include "PlayerStates.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"



void PlayerStates::Initialize([[maybe_unused]] GameEntity* _entity) {
   
   offSetY_ = 1.5f;
    bomExplotionNum_ = 0;
}

bool PlayerStates::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f);
    isChange |= ImGui::DragFloat("MoveRadius", &moveRadius_,0.01f);
    isChange |= ImGui::DragFloat("OffSetY", &offSetY_, 0.01f);
    return isChange;

}

void PlayerStates::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("moveSpeed", moveSpeed_);
    _writer.Write("moveRadius", moveRadius_);
}

void PlayerStates::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("moveSpeed", moveSpeed_);
    _reader.Read("moveRadius", moveRadius_);
}

void PlayerStates::Finalize() {}
