#include "SceneChangerSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/SceneChanger/SceneChangerStatus.h"
#include "component/ResultUI/ResultUIRankStatus.h"
#include "component/SceneTransition/SceneTransition.h"
#include "component/Timer/TimerStatus.h"
#include "engine/EngineInclude.h"
#include <Vector.h>

SceneChangerSystem::SceneChangerSystem() : ISystem(SystemType::StateTransition) {}

void SceneChangerSystem::Initialize() {
    input_        = Input::getInstance();
    sceneManager_ = SceneManager::getInstance();
}

void SceneChangerSystem::Finalize() {
}

SceneChangerSystem::~SceneChangerSystem() {}

void SceneChangerSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    gameEnd_ = getComponent<SceneChangerStatus>(_entity);

    AnimationChangeGameToResult();
    ResultTransition();
    TitleTransition();
    ///*シーン切り替え

    // タイトル
    ChangeSceneTitle();
    // リザルト
    ChangeSceneResult();
    // ゲーム
    ChangeSceneGame();
}

void SceneChangerSystem::ChangeSceneTitle() {
    if (gameEnd_->GetIsBackTitle()) {
        sceneManager_->changeScene("Title");
    }
}

void SceneChangerSystem::ChangeSceneResult() {
    if (gameEnd_->GetIsGoToResult()) {
        sceneManager_->changeScene("Result");
    }
}

void SceneChangerSystem::ChangeSceneGame() {
    if (gameEnd_->GetIsGoToGame()) {
        sceneManager_->changeScene("Game");
    }
}

void SceneChangerSystem::AnimationChangeGameToResult() {
    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* timerEntity                  = ecsManager->getUniqueEntity("Timer");
    GameEntity* GameToResultEntity           = ecsManager->getUniqueEntity("GameTransitionIN");

    if (!timerEntity || !GameToResultEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    TimerStatus* timerStatus          = getComponent<TimerStatus>(timerEntity);
    SceneTransition* transitionStatus = getComponent<SceneTransition>(GameToResultEntity);

    if (!transitionStatus || !timerStatus) {
        return;
    }

    // シーン遷移アニメーション切り替え
    if (timerStatus->GetCurrentTimer() <= 0.0f && transitionStatus->GetIsTransitionIn() == false) {
        transitionStatus->SetIsTransitionIn(true);
    }
}

void SceneChangerSystem::ResultTransition() {
    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* resultRankEntity             = ecsManager->getUniqueEntity("ResultRank");
    GameEntity* resultTransitionEntity       = ecsManager->getUniqueEntity("ResultTransitionIN");
    GameEntity* titleTransitionOutEntity     = ecsManager->getUniqueEntity("TitleOutTransition");

    if (!resultRankEntity || !resultTransitionEntity || !titleTransitionOutEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    ResultUIRankStatus* rankStatus         = getComponent<ResultUIRankStatus>(resultRankEntity);
    SceneTransition* transitionStatus      = getComponent<SceneTransition>(resultTransitionEntity);
    SceneTransition* titleTransitionStatus = getComponent<SceneTransition>(titleTransitionOutEntity);

    if (!transitionStatus || !rankStatus) {
        return;
    }

    if (rankStatus->GetAnimationStep() != RankAnimationStep::END) {
        return;
    }

    // retry
    if (input_->isTriggerKey(DIK_R)) {
        transitionStatus->SetIsTransitionIn(true);
        transitionStatus->SetIsRetry(true);
        //back titmle
    } else if (input_->isTriggerKey(DIK_T)) {
        transitionStatus->SetIsTransitionIn(true);
        titleTransitionStatus->SettIsTitleTransitionOut(true);
    }
}

void SceneChangerSystem::TitleTransition() {
    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* resultTransitionEntity       = ecsManager->getUniqueEntity("TitleTransitionIN");

    if (!resultTransitionEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    SceneTransition* transitionStatus = getComponent<SceneTransition>(resultTransitionEntity);

    if (!transitionStatus) {
        return;
    }

    if (input_->isTriggerKey(DIK_SPACE)) {
        transitionStatus->SetIsTransitionIn(true);
    }
}
