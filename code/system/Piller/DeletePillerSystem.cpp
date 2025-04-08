#include "DeletePillerSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/FloorAndPillerColum/FloorAndPillerrStatus.h"
#include "component/Piller/PillerStates.h"
#include "engine/EngineInclude.h"

DeletePillerSystem::DeletePillerSystem() : ISystem(SystemType::StateTransition) {}

void DeletePillerSystem::Initialize() {
}

void DeletePillerSystem::Finalize() {
    /*  entities_.clear();*/
}

DeletePillerSystem::~DeletePillerSystem() {}


void DeletePillerSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    PillerStates* status               = getComponent<PillerStates>(_entity);
   /* FloorAndPillerrStatus* fAndPStatus = getComponent<FloorAndPillerrStatus>(_entity);*/

    if (!status) {
        return;
    }

    // HPが0以下なら破壊処理
    if (status->GetCurrentHp() > 0) {
        return;
    }

    // 壊れた柱のRow番号を取得
    int destroyedRow = status->GetRowNum();

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

    // **壊れた柱を削除**
    DestroyEntity(_entity);


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
