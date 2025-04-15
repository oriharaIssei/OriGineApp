#include "FloorFallCollision.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void FloorFallCollision::Initialize([[maybe_unused]] GameEntity* _entity) {
    adaptTime_ = 0.1f;
}

bool FloorFallCollision::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    

    return isChange;
}

void FloorFallCollision::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
   
}

void FloorFallCollision::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    
}

void FloorFallCollision::Finalize() {}

void FloorFallCollision::TimeDecrement() {
    adaptTime_ -= Engine::getInstance()->getDeltaTime();
  }
