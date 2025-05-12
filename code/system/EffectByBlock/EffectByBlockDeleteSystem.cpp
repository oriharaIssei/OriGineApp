#include "EffectByBlockDeleteSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include

// component
#include "component/EffectByBlock/EffectByBlockUIStatus.h"

#include "engine/EngineInclude.h"


EffectByBlockDeleteSystem::EffectByBlockDeleteSystem()
    : ISystem(SystemType::StateTransition) {}

EffectByBlockDeleteSystem::~EffectByBlockDeleteSystem() {}

void EffectByBlockDeleteSystem::Initialize() {
}

void EffectByBlockDeleteSystem::Finalize() {}

void EffectByBlockDeleteSystem::UpdateEntity(GameEntity* _entity) {

    // get timer component
    effectByBlock_ = getComponent<EffectByBlockUIStatus>(_entity);

    if (!effectByBlock_) {
        return;
    }

    effectByBlock_->DecrementCurrnetTime(Engine::getInstance()->getDeltaTime());

    if (effectByBlock_->GetLifeTime() <= 0.0f) {
        DestroyEntity(_entity);
    }
   
}

void EffectByBlockDeleteSystem::ComboReset() {
}

float EffectByBlockDeleteSystem::Lerp(const float& start, const float& end, float t) {
    return (1.0f - t) * start + end * t;
}
