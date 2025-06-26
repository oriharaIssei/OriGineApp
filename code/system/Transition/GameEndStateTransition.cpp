#include "GameEndStateTransition.h"

/// engine
#define DELTA_TIME
#include "ECSManager.h"
#include "EngineInclude.h"
#include "sceneManager/SceneManager.h"

/// component
#include "component/SceneChanger.h"
#include "component/TimerComponent.h"
// system
#include "system/Input/PlayerInputSystem.h"
#include "system/Movement/MoveSystemByRigidBody.h"
#include "system/Movement/PlayerMoveSystem.h"

GameEndStateTransition::GameEndStateTransition()
    : ISystem(SystemType::StateTransition) {}

GameEndStateTransition::~GameEndStateTransition() {}

void GameEndStateTransition::Initialize() {
    isTransitioning_ = false;
}

void GameEndStateTransition::Finalize() {}

void GameEndStateTransition::UpdateEntity(GameEntity* _entity) {
    if (isTransitioning_) {
        auto sceneTransitionerEntity     = getUniqueEntity("SceneTransitioner");
        auto transitionEffectTimer   = getComponent<TimerComponent>(sceneTransitionerEntity);
        if (transitionEffectTimer) {
            if (transitionEffectTimer->getCurrentTime() <= 0.f) {
                // 遷移エフェクトのタイマーが終了したら、次のシーンに遷移
                auto sceneChanger = getComponent<SceneChanger>(sceneTransitionerEntity);
                SceneManager::getInstance()->changeScene(sceneChanger->getNextSceneName());
            }
        }
    } else {
        auto timerComponent = getComponent<TimerComponent>(_entity);
        if (timerComponent) {
            if (timerComponent->getCurrentTime() <= 0.f) {
                isTransitioning_ = true; // 遷移中フラグを立てる
                // Playerの操作を停止
                ECSManager* ecsManager = ECSManager::getInstance();
                ecsManager->StopSystem<PlayerInputSystem>(SystemType::Input);
                ecsManager->StopSystem<MoveSystemByRigidBody>(SystemType::Movement);
                ecsManager->StopSystem<PlayerMoveSystem>(SystemType::Movement);
                ecsManager->StopSystem<GameEndStateTransition>(SystemType::StateTransition);

                // SceneのTransitionEffect開始
                auto sceneTransitionerEntity = getUniqueEntity("SceneTransitioner");
                if (sceneTransitionerEntity) {
                    auto transitionEffect = getComponent<TimerComponent>(sceneTransitionerEntity);
                    if (transitionEffect) {
                        transitionEffect->setStarted(true);
                        transitionEffect->resetCurrentTime();

                        // transitionEffect を 開始する
                        // 使用されるSystem に 追加
                    }
                }
            }
        }
    }
}

void GameEndStateTransition::SettingsTransitionEffect(GameEntity* _effectEntity) {
    auto timerComponent = getComponent<TimerComponent>(_effectEntity);
    if (timerComponent) {
        timerComponent->setStarted(true);
        timerComponent->resetCurrentTime();
    }

};
