#include "GameEndSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/GameEnd/GameEnd.h"
#include "component/SceneTransition/SceneTransition.h"
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

    AnimationChangeGameToResult();

    ///*シーン切り替え

    // タイトル
    ChangeSceneTitle();
    // リザルト
    ChangeSceneResult();
    // ゲーム
    ChangeSceneGame();
}

void GameEndSystem::ChangeSceneTitle() {
    if (gameEnd_->GetIsBackTitle()) {
        sceneManager_->changeScene("TITLE");
    }
}

void GameEndSystem::ChangeSceneResult() {

    if (gameEnd_->GetIsGoToResult()) {
        sceneManager_->changeScene("Result");
    }
}

void GameEndSystem::ChangeSceneGame() {
    if (gameEnd_->GetIsGoToGame()) {
        sceneManager_->changeScene("GAME");
    }
}

void GameEndSystem::AnimationChangeGameToResult() {
    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* timerEntity                  = ecsManager->getUniqueEntity("Timer");
    GameEntity* GameToResultEntity           = ecsManager->getUniqueEntity("GameTransitionIN");

    if (!timerEntity || !GameToResultEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    TimerStatus* timerStatus = getComponent<TimerStatus>(timerEntity);
    SceneTransition* transitionStatus = getComponent<SceneTransition>(GameToResultEntity);

    if (!transitionStatus || !timerStatus) {
        return;
    }

    // シーン遷移アニメーション切り替え
    if (timerStatus->GetCurrentTimer() <= 0.0f && transitionStatus->GetIsTransitionIn()==false) {
        transitionStatus->SetIsTransitionIn(true);
    }
}
