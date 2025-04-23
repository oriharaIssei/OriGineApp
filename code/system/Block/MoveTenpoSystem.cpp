#include "MoveTenpoSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/Block/BlockManager.h"
#include "component/Timer/TimerStatus.h"
#include "engine/EngineInclude.h"
#include <Vector.h>

#include"KetaEasing.h"

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

    timer_ += Engine::getInstance()->getDeltaTime();
    curerntTenpoTime_ += Engine::getInstance()->getDeltaTime(); // easeTime

    blockManager_ = getComponent<BlockManager>(_entity);

    if (!blockManager_) {
        return;
    }

    blockManager_->SpeedChangeForTime(timer_);

    //イージングタイム更新
    blockManager_->ScalingEaseUpdate(Engine::getInstance()->getDeltaTime());

    // 1テンポ
    OneTenpoMethod();
}

void MoveTenpoSystem::OneTenpoMethod() {
    // 1テンポごとの処理
    if (curerntTenpoTime_ < blockManager_->GetMoveTenpo()) {
        return;
    }

    // 音鳴らす

    ///
    blockManager_->SetEaseType(EaseType::SCALING);
    blockManager_->ResetScalingEase();
    curerntTenpoTime_ = 0.0f;
    curentTempoNum_++; // テンポ加算

    //移動するべき場合
    if (curentTempoNum_ < blockManager_->GetMoveTenpoNum()) {
        return;
    }

    blockManager_->SetIsMove(true);
    blockManager_->SetEaseType(EaseType::MOVESCALING);
    curentTempoNum_ = 0;
}
