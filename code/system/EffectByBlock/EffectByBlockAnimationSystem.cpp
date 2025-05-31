#include "EffectByBlockAnimationSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include
#include <Vector3.h>
// component
#include "component/EffectByBlock/EffectByBlockAnimationStatus.h"
#include "component/Timer/TimerAnimationStatus.h"
#include "component/Timer/TimerStatus.h"

#include "engine/EngineInclude.h"
#include <Vector2.h>

EffectByBlockAnimationSystem::EffectByBlockAnimationSystem()
    : ISystem(SystemType::Movement) {}

EffectByBlockAnimationSystem::~EffectByBlockAnimationSystem() {}

void EffectByBlockAnimationSystem::Initialize() {
    time_           = 0.0f;
    scalingElapsed_ = 0.0f;
}

void EffectByBlockAnimationSystem::Finalize() {}

void EffectByBlockAnimationSystem::UpdateEntity(GameEntity* _entity) {

    // ComboEntityを取得
    /* EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
     GameEntity* timerEntity                  = ecsManager->getUniqueEntity("Timer");*/

    // if (!timerEntity) { // Entityが存在しない場合の早期リターン
    //     return;
    // }

    // get timer component
    EffectByBlockAnimationStatus* animationStatus = getComponent<EffectByBlockAnimationStatus>(_entity);
    Transform* transform                          = getComponent<Transform>(_entity);
    Rigidbody* rigitBody                          = getComponent<Rigidbody>(_entity);

    float deltaTIme = Engine::getInstance()->getDeltaTime();

    if (!animationStatus || !transform || !rigitBody) {
        return;
    }

    switch (animationStatus->GetAnimationStep()) {
        ///----------------------------------------------------------------
        /// Move Animation
        ///----------------------------------------------------------------
    case EffectByBlockAnimationStatus::AnimationStep::START:
        // アニメーションリセット
        animationStatus->Reset();
        animationStatus->SetAnimationStep(EffectByBlockAnimationStatus::AnimationStep::APEAR);
        break;
        ///----------------------------------------------------------------
        /// Move Animation
        ///----------------------------------------------------------------
    case EffectByBlockAnimationStatus::AnimationStep::APEAR:

        animationStatus->ApearEasing(deltaTIme);

        break;
        ///----------------------------------------------------------------
        /// Move Animation
        ///----------------------------------------------------------------
    case EffectByBlockAnimationStatus::AnimationStep::WAIT:
        animationStatus->FadeOutWaitUpdate(deltaTIme, transform->translate[X]);

        break;
        ///----------------------------------------------------------------
        /// Scroll Wait
        ///----------------------------------------------------------------
    case EffectByBlockAnimationStatus::AnimationStep::FADEOUT:

        animationStatus->FadeOutEasing(deltaTIme);

        break;
        ///----------------------------------------------------------------
        /// Scroll Animation
        ///----------------------------------------------------------------
    case EffectByBlockAnimationStatus::AnimationStep::END:

        break;
    default:
        break;
    }

    transform->scale = animationStatus->GetBaseScale();


    // fade out
    if (animationStatus->GetAnimationStep() != EffectByBlockAnimationStatus::AnimationStep::FADEOUT) {
        return;
    }

    if (animationStatus->GetBasePosX() == 0.0f) {
        return;
    }

    rigitBody->setVelocity(Vec3f(0.0f, 0.0f, 0.0f));
    transform->translate[X] = animationStatus->GetBasePosX();

      // Quaternionに変換
    Quaternion q = Quaternion::FromEulerAngles(0.0f, 0.0f, animationStatus->GetRotate());
    // クォータニオンで回転をセット
    transform->rotate = q;

}
