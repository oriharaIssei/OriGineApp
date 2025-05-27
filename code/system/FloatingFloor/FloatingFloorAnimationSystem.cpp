#include "FloatingFloorAnimationSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
// ECS
#define ENGINE_ECS
// include

// component
#include "component/Piller/FloatingFloorAnimationStatus.h"
#include "component/Piller/FloatingFloorStatus.h"

#include "engine/EngineInclude.h"

FloatingFloorAnimationSystem::FloatingFloorAnimationSystem()
    : ISystem(SystemType::Movement) {}

FloatingFloorAnimationSystem::~FloatingFloorAnimationSystem() {}

void FloatingFloorAnimationSystem::Initialize() {
    time_ = 0.0f;
}

void FloatingFloorAnimationSystem::Finalize() {}

void FloatingFloorAnimationSystem::UpdateEntity(GameEntity* _entity) {

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* animationEntity              = ecsManager->getUniqueEntity("FloatingFloorAnimation");

    if (!animationEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    animationStatus_                         = getComponent<FloatingFloorAnimationStatus>(animationEntity);
    FloatingFloorStatus* floatingFloorStatus = getComponent<FloatingFloorStatus>(_entity);
    Transform* transform                     = getComponent<Transform>(_entity);

    if (!animationStatus_ || !floatingFloorStatus || !transform) {
        return;
    }

    float deltaTime = Engine::getInstance()->getDeltaTime();

    switch (floatingFloorStatus->GetReactionStep()) {
        ///---------------------------------------------------
        /// NONE
        ///---------------------------------------------------
    case ReactionStep::NONE:

        if (!floatingFloorStatus->GetIsDamageShake()) {
            break;
        }
        floatingFloorStatus->DamageShakeReset(animationStatus_);

        floatingFloorStatus->SetReactionStep(ReactionStep::DAMAGESHAKE);
        break;

        ///---------------------------------------------------
        /// ダメージシェイク
        ///---------------------------------------------------
    case ReactionStep::DAMAGESHAKE:

        floatingFloorStatus->DamageShake(animationStatus_->GetShakeValue(), deltaTime);
        break;

        ///---------------------------------------------------
        /// 常に揺れる
        ///---------------------------------------------------
    case ReactionStep::CONSTANTSHAKE:
        floatingFloorStatus->ConstantShake(animationStatus_->GetConstantShakeValue());
        break;

        ///---------------------------------------------------
        /// 終わり
        ///---------------------------------------------------
    case ReactionStep::END:
        floatingFloorStatus->DamageShakeReset(animationStatus_);
      /*  floatingFloorStatus->SetReactionStep(ReactionStep::NONE);*/
        break;
    default:
        break;
    }

    //復活演出
    floatingFloorStatus->RevivalAnimation(deltaTime, animationStatus_->GetRevivalMaxTime());

    transform->translate[X] = floatingFloorStatus->GetSavePos()[X] + floatingFloorStatus->GetDamageShakePos()[X];
    transform->scale        = floatingFloorStatus->GetBaseScale();
}

void FloatingFloorAnimationSystem::ComboReset() {
}
