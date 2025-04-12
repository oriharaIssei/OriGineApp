#include "CanageStateFallSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
// component
#include "component/Piller/PillerStatus.h"
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

    FloorAndPillerrStatus* fAndPStatus = getComponent<FloorAndPillerrStatus>(_entity);

    if (!fAndPStatus) {
        return;
    }
  

    // HPが0以下なら破壊処理
    if (fAndPStatus->GetCurrentHP() > 0 ||fAndPStatus->GetColumNum()<0) {
        return;
    }

    // コンボ加算
    ComboCountIncrement();

    //  柱のスケールを0にする
    Transform* entityTransform = getComponent<Transform>(_entity, 0);
    if (!entityTransform) {
        return;
    }
    entityTransform->scale = Vec3f(0.0f, 0.0f, 0.0f);/// スケールセロ

    // 壊れた柱のRow番号を取得
    int destroyedRow = fAndPStatus->GetRowNum();

    // **同じRowの床を落とす**
    for (GameEntity* entity : getEntities()) { // すべてのエンティティをチェック
        FloorAndPillerrStatus* otherFAndP = getComponent<FloorAndPillerrStatus>(entity);
        if (!otherFAndP) {
            continue;
        }

        // 同じRowにある床の `IsFall` を `true` にする
        if (otherFAndP->GetRowNum() == destroyedRow) {
            otherFAndP->SetIsFall(true);
        }
    }

    //// **壊れた柱を削除**
    //DestroyEntity(_entity);


    //// 壊れた柱のRow番号を取得
    //int destroyedRow = fAndPStatus->GetRowNum();

   /* ComponentArray<FloorAndPillerrStatus>* fandp = ECSManager::getInstance()->getComponentArray<FloorAndPillerrStatus>();*/

    //if (fandp == nullptr) {
    //    return;
    //}
    // 
    //FloorAndPillerrStatus* otherFAndP = ;

    //// 同じRowにある床の `IsFall` を `true` にする
    //if (fandp->GetRowNum() == destroyedRow) {
    //    otherFAndP->SetIsFall(true);
    //}

    //// **壊れた柱を削除**
    //DestroyEntity(_entity);
}

void CanageStateFallSystem::ComboCountIncrement() {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* ComboEntity                  = ecsManager->getUniqueEntity("Combo");

    if (!ComboEntity) {
        return;
    }

    ComboStatus* comboStatus = getComponent<ComboStatus>(ComboEntity);

    if (!comboStatus) {
        return;
    }

    comboStatus->SetComboIncrement(); // 現在のコンボ数をインクリメント
}
