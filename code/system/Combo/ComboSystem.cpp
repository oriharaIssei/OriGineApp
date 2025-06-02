#include "ComboSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
// lib

// include

// component
#include "component/Combo/ComboStatus.h"
#include"component/Timer/TimerStatus.h"




ComboSystem::ComboSystem()
    : ISystem(SystemType::Movement) {}

ComboSystem::~ComboSystem() {}

void ComboSystem::Initialize() {
}

void ComboSystem::Finalize() {}

void ComboSystem::UpdateEntity(GameEntity* _entity) {

   comboStatus_ = getComponent<ComboStatus>(_entity);

    if (!comboStatus_) {
        return;
    }

    if (!comboStatus_->GetIsUpdateCombo()) {//コンボ更新中じゃない時の早期リターン
        return;
    }

    ///　コンボアニメーションとか追加

    //タイム減算、0より大きい場合の早期リターン
    comboStatus_->SetCurerntTimeDecrement(Engine::getInstance()->getDeltaTime());
    if (comboStatus_->GetCurrentTimer() > 0.0f) {
        return;
    }

    TimeUpdate();

    //コンボリセット
    ComboReset();

   
}

void ComboSystem::ComboReset() {

    if ((comboStatus_->GetMaxComboNum() <= comboStatus_->GetMaxComboNum())) {
        comboStatus_->SetMaxComboNum(comboStatus_->GetCurrentComboNum()); // MAXコンボをセット
    }

    comboStatus_->SetCurrentCombo(0);
    comboStatus_->SetCurrentTime(0.0f);
    comboStatus_->SetIsUpdateCombo(false);
 }

void ComboSystem::TimeUpdate() {
     // ComboEntityを取得
     EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
     GameEntity* TimerEntity                  = ecsManager->getUniqueEntity("Timer");

     if (!TimerEntity) { // Entityが存在しない場合の早期リターン
         return;
     }

      /// component取得
     TimerStatus* timeStatus     = getComponent<TimerStatus>(TimerEntity);
   
     if (!timeStatus) { // Componentが存在しない場合の早期リターン
         return;
     }

     timeStatus->CurrentTimeUpdate(comboStatus_->GetCurrentComboNum());
   
 }
