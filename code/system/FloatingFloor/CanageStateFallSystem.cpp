#include "CanageStateFallSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
// component
#include "component/Piller/FloatingFloorStatus.h"
#include"component/Combo/ComboStatus.h"

CanageStateFallSystem::CanageStateFallSystem() : ISystem(SystemType::StateTransition) {}

void CanageStateFallSystem::Initialize() {
}

void CanageStateFallSystem::Finalize() {
    /*  entities_.clear();*/
}

CanageStateFallSystem::~CanageStateFallSystem() {}


void CanageStateFallSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();

    FloatingFloorStatus* fAndPStatus = getComponent<FloatingFloorStatus>(_entity);
    GameEntity* FloorSound            = ecsManager->getUniqueEntity("FloorSound");
    Audio* fallSound                 = getComponent<Audio>(FloorSound, 0); // 落ちる音

    if (!fAndPStatus || !fallSound) {
        return;
    }
  

    // 破壊処理
    if (fAndPStatus->GetCurrentHP() > 0 ||fAndPStatus->GetIsFall()) {
        return;
    }

    //死亡中は通らない
    if (fAndPStatus->GetIsDestroy()) {
        return;
    }

    fallSound->Play();//再生
    fAndPStatus->SetIsFall(true);

   
     // コンボ加算
   /* ComboCountIncrement();*/
}

void CanageStateFallSystem::ComboCountIncrement() {

    //// ComboEntityを取得
    //EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    //GameEntity* ComboEntity                  = ecsManager->getUniqueEntity("Combo");

    //if (!ComboEntity) {
    //    return;
    //}

    //ComboStatus* comboStatus = getComponent<ComboStatus>(ComboEntity);

    //if (!comboStatus) {
    //    return;
    //}

    //comboStatus->SetCurrentTime(comboStatus->GetContinuationTime());
    //comboStatus->SetComboIncrement(); // 現在のコンボ数をインクリメント
    //comboStatus->SetIsUpdateCombo(true); // コンボ更新フラグを立てる
}
