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
    sceneManager_ = SceneManager::getInstance();
}

void GameEndSystem::Finalize() {
}

GameEndSystem::~GameEndSystem() {}

void GameEndSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

     gameEnd_ = getComponent<GameEnd>(_entity);

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* timerEntity                  = ecsManager->getUniqueEntity("Timer");

    if (!timerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    TimerStatus* timerStatus = getComponent<TimerStatus>(timerEntity);

    //タイトル
    ChangeSceneTitle();
    //リザルト
    ChangeSceneResult(timerStatus);   
}

void GameEndSystem::ChangeSceneTitle() {
    if (gameEnd_->GetIsBackTitle()) {
        sceneManager_->changeScene("Title");
    }
  }

void GameEndSystem::ChangeSceneResult(TimerStatus* timerstauts) {
    // タイマーゼロでゲームエンド
    if (timerstauts->GetCurrentTimer() <= 0.0f) {
        gameEnd_->SetIsGameEnd(true);
    }

    if (gameEnd_->GetIsGameEnd()) {
        sceneManager_->changeScene("Result");
    }
 }
