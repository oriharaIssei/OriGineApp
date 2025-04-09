#include "CanageStateFallSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/FloorAndPillerColum/FloorAndPillerrStatus.h"
#include "component/Piller/PillerStates.h"
#include "engine/EngineInclude.h"

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

    PillerStates* status               = getComponent<PillerStates>(_entity);
    FloorAndPillerrStatus* fAndPStatus = getComponent<FloorAndPillerrStatus>(_entity);

    if (!status) {
        return;
    }
    if (is) {
        return;
    }
    if (fAndPStatus->GetRowNum() == 0 && fAndPStatus->GetColumNum() == 0) {
        status->SetCurrentHp(0);
        is = true;
    }

    // HPが0以下なら破壊処理
    if (status->GetCurrentHp() > 0) {
        return;
    }

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
