#include "PlayerInput.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
#include "input/Input.h"
// include
#include <Quaternion.h>
#include <Vector3.h>
// component
#include "component/Bom/BomSpawner.h"
#include "component/Player/PlayerStates.h"
#include"component/Menu/MenuStatus.h"
#include"component/GameEndUI/GameEndUIStatus.h"
#include"component/Player/PlayerAnimationStatus.h"
#include"component/GameStart/GameStartStatus.h"

#include "engine/EngineInclude.h"
#include <cmath>
#include <numbers>

PlayerInputSystem::PlayerInputSystem()
    : ISystem(SystemType::Input) {}

PlayerInputSystem::~PlayerInputSystem() {}

void PlayerInputSystem::Initialize() {
    isInited_ = false;
    input_    = Input::getInstance();
}

void PlayerInputSystem::Finalize() {}

void PlayerInputSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    // ポーズ中は通さない
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* menuEntity           = ecsManager->getUniqueEntity("Menu");
    GameEntity* gameStartEntity              = ecsManager->getUniqueEntity("GameStartRendition");



    if (!menuEntity || !gameStartEntity) {
        return;
    }

     MenuStatus* menu = getComponent<MenuStatus>(menuEntity);
    GameStartStatus* gameStartStatus = getComponent<GameStartStatus>(gameStartEntity);


     if (!menu) {
         return;
     }

     if (menu->GetIsPose()) {
         return;
     }

     if (!gameStartStatus->GetIsStart()) {
         return;
     }
   
     GameEntity* gameEndUIEntity                  = ecsManager->getUniqueEntity("GameEndUI");

     if (!gameEndUIEntity) { // Entityが存在しない場合の早期リターン
         return;
     }

     // get timer component
     GameEndUIStatus* gameEndUIStatus = getComponent<GameEndUIStatus>(gameEndUIEntity);
    
     if (gameEndUIStatus->GetAnimationStep() != GameEndUIStep::NONE) {
         return;
     }

    ///============================================================
    // 必要なコンポーネントを取得
    ///============================================================
   

    entityPlayerStates_ = getComponent<PlayerStates>(_entity);
     PlayerAnimationStatus* playerAnimationStatus = getComponent<PlayerAnimationStatus>(_entity);
    if (!entityPlayerStates_) {
        return;
    }

    TransformInit(_entity);

    ///================================================================
    // 爆弾の設置入力
    ///================================================================
    PutBom(_entity);

    ///============================================================
    // 円運動の更新（Quaternionベース）
    ///============================================================
   
    float inputDirection = 0.0f;

    /// 入力で向き決定
    if (input_->isPressKey(DIK_D)) {
        inputDirection = 1.0f; // 反時計回り
    }
    if (input_->isPressKey(DIK_A)) {
        inputDirection -= 1.0f; // 時計回り
    }

      entityPlayerStates_->SetDirection(inputDirection);

         // アニメーション切り替え
      if (inputDirection != 0) {
          playerAnimationStatus->ChangeMotion(PlayerAnimationStatus::MotionStep::MOVE);
      } else {
       
          playerAnimationStatus->ChangeMotion(PlayerAnimationStatus::MotionStep::WAIT);
      }
     

}

void PlayerInputSystem::TransformInit(GameEntity* _entity) {
    if (_entity) {
        return;
    }

  
}

void PlayerInputSystem::PutBom(GameEntity* _entity) {

     // PlayerEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* playerEntity                 = ecsManager->getUniqueEntity("Player");

   
    if (!playerEntity) {
        return;
    }

    PlayerStates* playerStates = getComponent<PlayerStates>(playerEntity);

    if (playerStates->GetIsBigBomHaving()) {
        return;
    }

    BomSpawner* entityBomSpawner = getComponent<BomSpawner>(_entity);

    if (!entityBomSpawner) {
        return;
    }

    // 発射済み＆BigBomがある場合
    if (playerStates->GetIsBigBomHaving()||entityBomSpawner->GetIsLaunched()) {
        return;
    }

    // タイム取得
    float currentCoolTime = entityBomSpawner->GetCurrenPutCoolTime();
    entityBomSpawner->SetIsLaunch(false);

    // coolTimeが0以下になったら発射
    if (currentCoolTime <= 0.0f) {

        entityBomSpawner->SetIsLaunch(input_->isTriggerKey(DIK_SPACE));
     

    } else {
        // coolTimeを減らす
        currentCoolTime -= Engine::getInstance()->getDeltaTime();
        entityBomSpawner->SetPutCurrentCoolTime((std::max)(currentCoolTime, 0.f));
    }
}
