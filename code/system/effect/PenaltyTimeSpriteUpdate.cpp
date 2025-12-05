#include "PenaltyTimeSpriteUpdate.h"

/// component
#include "component/animation/SpriteAnimation.h"
#include "component/TimerComponent.h"

/// math
#include "math/MyEasing.h"

PenaltyTimeSpriteUpdate::PenaltyTimeSpriteUpdate() : ISystem(SystemCategory::Effect) {}
PenaltyTimeSpriteUpdate::~PenaltyTimeSpriteUpdate() {}

void PenaltyTimeSpriteUpdate::Initialize() {}
void PenaltyTimeSpriteUpdate::Finalize() {}

void PenaltyTimeSpriteUpdate::UpdateEntity(Entity* _entity) {
    auto timerComp = GetComponent<TimerComponent>(_entity);

    // TimerComponentが存在しない場合は処理しない
    if (!timerComp) {
        return;
    }
    // SpriteAnimationが存在しない場合は処理しない
    auto spriteAnimaiton = GetComponent<SpriteAnimation>(_entity);
    if (!spriteAnimaiton) {
        return;
    }

    // アニメーション終了したらエンティティを削除
    if (spriteAnimaiton->IsEnded()) {
        GetScene()->AddDeleteEntity(_entity->GetID());
    };
}
