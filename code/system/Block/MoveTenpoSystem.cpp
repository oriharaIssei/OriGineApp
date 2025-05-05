#include "MoveTenpoSystem.h"

/// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
// component
#include "component/Block/BlockManager.h"
#include "component/Block/BlockStatus.h"
#include"component/LevelUPUI/LevelUIParentStatus.h"
#include"component/Menu/MenuStatus.h"
#include <Vector.h>

#include "KetaEasing.h"

MoveTenpoSystem::MoveTenpoSystem() : ISystem(SystemType::Movement) {}

void MoveTenpoSystem::Initialize() {
    timer_ = 0.0f;
}

void MoveTenpoSystem::Finalize() {
    /*entities_.clear();*/
}

MoveTenpoSystem::~MoveTenpoSystem() {}

void MoveTenpoSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }



       // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* levelUI                  = ecsManager->getUniqueEntity("LevelUIParent");
    GameEntity* menuEntity                   = ecsManager->getUniqueEntity("Menu");

    if (!levelUI || !menuEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    // ポーズ中は通さない
    MenuStatus* menu = getComponent<MenuStatus>(menuEntity);

    if (!menu) {
        return;
    }

    if (menu->GetIsPose()) {
        return;
    }

    timer_ += Engine::getInstance()->getDeltaTime();
    curerntTenpoTime_ += Engine::getInstance()->getDeltaTime(); // easeTime

    blockManager_ = getComponent<BlockManager>(_entity);
    LevelUIParentStatus* levelUIParentStatus = getComponent<LevelUIParentStatus>(levelUI);

    if (!blockManager_ || !levelUIParentStatus) {
        return;
    }
   
    blockManager_->SpeedChangeForTime(timer_, levelUIParentStatus);
    blockManager_->SetMoveTempoForLevel();

    // イージングタイム更新
    blockManager_->ScalingEaseUpdate(Engine::getInstance()->getDeltaTime());

    blockManager_->SetIsMove(false);
    // 1テンポ
    OneTenpoMethod(_entity);
}

void MoveTenpoSystem::OneTenpoMethod(GameEntity* _entity) {
    // 1テンポごとの処理
    if (curerntTenpoTime_ < blockManager_->GetMoveTenpo()) {
        return;
    }

    // 音鳴らす
    Audio* audio = getComponent<Audio>(_entity);

    ///
    blockManager_->SetEaseType(EaseType::SCALING);
    blockManager_->ResetScalingEase();
    curerntTenpoTime_ = 0.0f;
    curentTempoNum_++; // テンポ加算

    // 移動するべき場合
    if (curentTempoNum_ < blockManager_->GetMoveTenpoNum()) {

        audio->Play();//テンポ音
        return;
    }

    blockManager_->SetIsMove(true);
    blockManager_->SetEaseType(EaseType::MOVESCALING);
    curentTempoNum_ = 0;

    // 動く音
    Audio* audio2 = getComponent<Audio>(_entity, 1);
    audio2->Play();

    // for (GameEntity* entity : getEntities()) { // すべてのエンティティをチェック
    //     BlockStatus* block = getComponent<BlockStatus>(entity);
    //     Transform* transform    = getComponent<Transform>(entity);

    //    if (!block || !transform) {
    //        continue;
    //    }

    //    // 座標取得
    //    block->SetPreMovePos(transform->translate);
    //    block->SetIsMove(true);
    //}
}
