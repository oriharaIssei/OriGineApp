#include "PlayerAnimationSystem.h"

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
#include "component/Block/BlockManager.h"
#include "component/Piller/FloatingFloorStatus.h"
#include "component/Player/PlayerAnimationStatus.h"
#include "component/Player/PlayerStates.h"

#include "engine/EngineInclude.h"

PlayerAnimationSystem::PlayerAnimationSystem()
    : ISystem(SystemType::Movement) {}

PlayerAnimationSystem::~PlayerAnimationSystem() {}

void PlayerAnimationSystem::Initialize() {
    time_ = 0.0f;
}

void PlayerAnimationSystem::Finalize() {}

void PlayerAnimationSystem::UpdateEntity(GameEntity* _entity) {

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* blockManagerEntity           = ecsManager->getUniqueEntity("BlockManager");

    if (!blockManagerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    PlayerAnimationStatus* playerAnimationStatus = getComponent<PlayerAnimationStatus>(_entity);
    Transform* transform                         = getComponent<Transform>(_entity, 0);
    PlayerStates* playerStatus                   = getComponent<PlayerStates>(_entity);
    BlockManager* blockStatus                    = getComponent<BlockManager>(blockManagerEntity);
    Audio* moveAudio                             = getComponent<Audio>(_entity,2);

    if (!playerAnimationStatus || !transform) {
        return;
    }

    float deltaTime = Engine::getInstance()->getDeltaTime();

    switch (playerAnimationStatus->GetPlayerMotionStep()) {
        ///---------------------------------------------------
        /// NONE
        ///---------------------------------------------------
    case PlayerAnimationStatus::MotionStep::WAIT:
        playerAnimationStatus->WaitAnimation(deltaTime, blockStatus->GetResultScale());
        playerAnimationStatus->LandingAnimation(deltaTime);
        break;

        ///---------------------------------------------------
        /// ダメージシェイク
        ///---------------------------------------------------
    case PlayerAnimationStatus::MotionStep::MOVE:
        playerAnimationStatus->MoveAnimaiton(deltaTime, moveAudio);
        playerAnimationStatus->LandingAnimation(deltaTime);

        break;

        ///---------------------------------------------------
        /// 常に揺れる
        ///---------------------------------------------------
    case PlayerAnimationStatus::MotionStep::LAUNCH:
        playerAnimationStatus->LaunchScaleAnimation(deltaTime);
        playerAnimationStatus->LaunchRotateAnimation(deltaTime);
        playerAnimationStatus->MoveAnimaiton(deltaTime, moveAudio);
        playerAnimationStatus->LandingAnimation(deltaTime);
        playerAnimationStatus->ChangeMotionWait();
        break;

    default:
        break;
    }

    // transform
    transform->translate[Y] = playerAnimationStatus->GetBaseYOffset() + playerAnimationStatus->GetJumpPosY();
    transform->scale        = playerAnimationStatus->GetBaseScale() + playerAnimationStatus->GetAnimationScale() + playerAnimationStatus->GetLaunchScale();

    // rotate

    // Quaternionに変換
    Quaternion animationRotation = Quaternion::FromEulerAngles(0.0f, playerAnimationStatus->GetLaunchRotateY(), 0.0f);

    Quaternion baseRotation = playerStatus->GetPartsHeadRotate(); // MoveSystemで設定された回転
    transform->rotate       = baseRotation * animationRotation;
}

void PlayerAnimationSystem::ComboReset() {
}
