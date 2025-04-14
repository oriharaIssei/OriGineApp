#include "ComboSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include
#include <Quaternion.h>
#include <Vector3.h>
// component
#include "component/Combo/ComboStatus.h"
#include "component/Combo/ComboUIStatus.h"

#include "engine/EngineInclude.h"
#include <cmath>
#include <cstdint>

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
