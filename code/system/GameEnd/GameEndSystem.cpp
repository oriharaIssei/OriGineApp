#include "GameEndSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/GameEnd/GameEnd.h"
#include "component/Timer/TimerStatus.h"
#include "engine/EngineInclude.h"
#include <Vector.h>


GameEndSystem::GameEndSystem() : ISystem(SystemType::StateTransition) {}

void GameEndSystem::Initialize() {
}

void GameEndSystem::Finalize() {
    sceneManager_ = SceneManager::getInstance();
}

GameEndSystem::~GameEndSystem() {}

void GameEndSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    GameEnd* gameEnd = getComponent<GameEnd>(_entity);

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* timerEntity                  = ecsManager->getUniqueEntity("Timer");

    if (!timerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    TimerStatus* timerStatus = getComponent<TimerStatus>(timerEntity);

    // タイマーゼロでゲームエンド
    if (timerStatus->GetCurrentTimer() <= 0.0f) {
        gameEnd->SetIsGameEnd(true);
    }


    if (!gameEnd->GetIsGameEnd()) {
        return;
    }

    //タイトル
    if (Input::getInstance()->isTriggerKey(DIK_T)) {
    
        sceneManager_->changeScene("TITLE");
    }
}
