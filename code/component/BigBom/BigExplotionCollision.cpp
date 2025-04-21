#include "BigExplotionCollision.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void BigExplotionCollision::Initialize([[maybe_unused]] GameEntity* _entity) {
    adaptTime_ = 0.1f;
}

bool BigExplotionCollision::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

      ImGui::Text("Offset");
    isChange |= ImGui::InputFloat3("##BomOffset", positionOffset_.v);

      ImGui::Text("etc");
    isChange |= ImGui::DragFloat("adaptTime", &adaptTime_);
    isChange |= ImGui::DragFloat("CollisionRadius", &collisionRadius_);

    return isChange;
}

void BigExplotionCollision::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("adaptTime", adaptTime_);
    _writer.Write<3, float>("positionOffset", positionOffset_);
    _writer.Write("CollisionRadius", collisionRadius_);
}

void BigExplotionCollision::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("adaptTime", adaptTime_);
    _reader.Read<3, float>("positionOffset", positionOffset_);
    _reader.Read("CollisionRadius", collisionRadius_);
}

void BigExplotionCollision::Finalize() {}

void BigExplotionCollision::TimeDecrement() {
    adaptTime_ -= Engine::getInstance()->getDeltaTime();
  }
