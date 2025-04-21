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

    FloatingFloorStatus* fAndPStatus = getComponent<FloatingFloorStatus>(_entity);

    if (!fAndPStatus) {
        return;
    }
  

    // HPが0以下なら破壊処理（ここ一回しか通らないように）
    if (fAndPStatus->GetCurrentHP() > 0 ||fAndPStatus->GetIsFall()) {
        return;
    }

    if (fAndPStatus->GetIsDestroy()) {
        return;
    }

    fAndPStatus->SetIsFall(true);

    ////  柱のスケールを0にする
    //Transform* entityTransform = getComponent<Transform>(_entity, 0);
    //if (!entityTransform) {
    //    return;
    //}
    //entityTransform->scale = Vec3f(0.0f, 0.0f, 0.0f);/// スケールセロ

    //// 壊れた柱のRow番号を取得
    //int destroyedRow = fAndPStatus->GetRowNum();

    //// **同じRowの床を落とす**
    //for (GameEntity* entity : getEntities()) { // すべてのエンティティをチェック
    //    FloatingFloorStatus* otherFAndP = getComponent<FloatingFloorStatus>(entity);
    //    if (!otherFAndP) {
    //        continue;
    //    }

    //    // 同じRowにある床の `IsFall` を `true` にする
    //    if (otherFAndP->GetRowNum() == destroyedRow) {
    //        otherFAndP->SetIsFall(true);
    //    }
    //}

     // コンボ加算
    ComboCountIncrement();
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

    comboStatus->SetCurrentTime(comboStatus->GetContinuationTime());
    comboStatus->SetComboIncrement(); // 現在のコンボ数をインクリメント
    comboStatus->SetIsUpdateCombo(true); // コンボ更新フラグを立てる
}
