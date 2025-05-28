#include "GameStartRenditionSystem.h"

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
#include "component/GameStart/GameStartStatus.h"
#include"component/Menu/MenuStatus.h"

#include "engine/EngineInclude.h"

GameStartRenditionSystem::GameStartRenditionSystem()
    : ISystem(SystemType::Movement) {}

GameStartRenditionSystem::~GameStartRenditionSystem() {}

void GameStartRenditionSystem::Initialize() {
    time_ = 0.0f;
}

void GameStartRenditionSystem::Finalize() {}

void GameStartRenditionSystem::UpdateEntity(GameEntity* _entity) {


    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* menuEntity                   = ecsManager->getUniqueEntity("Menu");
  
    GameStartStatus* gameStartStatus = getComponent<GameStartStatus>(_entity);
    Audio* purposeApearAudio           = getComponent<Audio>(_entity);
    Audio* readyAudio         = getComponent<Audio>(_entity,1);
    Audio* goAudio           = getComponent<Audio>(_entity,2);
    MenuStatus* menu                 = getComponent<MenuStatus>(menuEntity);

    if (!gameStartStatus || !menuEntity) {
        return;
    }

    if (menu->GetIsPose()) {
        return;
    }

    float deltaTime = Engine::getInstance()->getDeltaTime();

    switch (gameStartStatus->GetRenditionStep()) {
        ///---------------------------------------------------
        /// 最初の待機
        ///---------------------------------------------------
    case GameStartStatus::RenditionStep::INIT:

     gameStartStatus->Reset();
        gameStartStatus->SetRenditionStep(GameStartStatus::RenditionStep::WAIT);

        break;
        ///---------------------------------------------------
        /// 最初の待機
        ///---------------------------------------------------
    case GameStartStatus::RenditionStep::WAIT:

        time_ += deltaTime;
        if (time_ < gameStartStatus->GetFirstWaitTime()) {
            return;
        }
        purposeApearAudio->Play();
        time_ = 0.0f;
        gameStartStatus->SetRenditionStep(GameStartStatus::RenditionStep::APEARPURPOSE);

        break;
        ///---------------------------------------------------
        /// 目的出現
        ///---------------------------------------------------
    case GameStartStatus::RenditionStep::APEARPURPOSE:
        gameStartStatus->SetIsBackApear(true);
        gameStartStatus->ApearEasing(deltaTime);
        break;
        ///---------------------------------------------------
        /// 2回目の待機
        ///---------------------------------------------------
    case GameStartStatus::RenditionStep::WAITAFTERAPERAR:
        time_ += deltaTime;
        if (time_ < gameStartStatus->GetWaitTimeAfterApear()) {
            return;
        }
        time_ = 0.0f;
        gameStartStatus->SetRenditionStep(GameStartStatus::RenditionStep::CLOSE);

        break;
        ///---------------------------------------------------
        /// 目的閉じる
        ///---------------------------------------------------
    case GameStartStatus::RenditionStep::CLOSE:
        gameStartStatus->CloseEasing(deltaTime);
        break;
        ///---------------------------------------------------
        /// NONE
        ///---------------------------------------------------
    case GameStartStatus::RenditionStep::WAITAFTERCLOSE:
        gameStartStatus->SetIsBackApear(false);
        time_ += deltaTime;
        if (time_ < gameStartStatus->GetWaitTimeAfterClose()) {
            return;
        }
        time_ = 0.0f;
        readyAudio->Play();
        gameStartStatus->SetRenditionStep(GameStartStatus::RenditionStep::READY);
        break;
        ///---------------------------------------------------
        /// NONE
        ///---------------------------------------------------
    case GameStartStatus::RenditionStep::READY:
        gameStartStatus->ReadyEasing(deltaTime);
        break;
        ///---------------------------------------------------
        /// NONE
        ///---------------------------------------------------
    case GameStartStatus::RenditionStep::GO:
        gameStartStatus->GoEasing(deltaTime,goAudio);
        break;
        ///---------------------------------------------------
        /// NONE
        ///---------------------------------------------------
    case GameStartStatus::RenditionStep::WAITAFTERGO:
        time_ += deltaTime;
        if (time_ < gameStartStatus->GetWaitTimeAfterGo()) {
            return;
        }
        time_ = 0.0f;
        gameStartStatus->SetRenditionStep(GameStartStatus::RenditionStep::GOBACK);
        break;
        ///---------------------------------------------------
        /// NONE
        ///---------------------------------------------------
    case GameStartStatus::RenditionStep::GOBACK:
        gameStartStatus->SetIsStart(true);
        gameStartStatus->GoBackEasing(deltaTime);
        break;
        ///---------------------------------------------------
        /// NONE
        ///---------------------------------------------------
    case GameStartStatus::RenditionStep::END:

        break;
    default:
        break;
    }
}

void GameStartRenditionSystem::ComboReset() {
}
