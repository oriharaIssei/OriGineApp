#include "PenaltySystem.h"

/// engine
#include "Engine.h"
#include "scene/Scene.h"
#include "winApp/WinApp.h"

/// component
#include "component/animation/MaterialAnimation.h"
#include "component/animation/SpriteAnimation.h"
#include "component/player/state/PlayerState.h"
#include "component/renderer/Sprite.h"
#include "component/TimerComponent.h"

/// system
#include "system/SystemRunner.h"

#include "system/effect/PenaltyTimeSpriteUpdate.h"
#include "system/effect/SpriteAnimationSystem.h"
#include "system/effect/TimerForSprite.h"

/// math
#include "math/mathEnv.h"

using namespace OriGine;

PenaltySystem::PenaltySystem() : ISystem(OriGine::SystemCategory::StateTransition) {}
PenaltySystem::~PenaltySystem() {}

void PenaltySystem::Initialize() {
    createSpriteFromTimerSystem_ = std::make_unique<CreateSpriteFromTimer>();
    createSpriteFromTimerSystem_->SetScene(GetScene());
    createSpriteFromTimerSystem_->Initialize();
}
void PenaltySystem::Finalize() {
    createSpriteFromTimerSystem_->Finalize();
    createSpriteFromTimerSystem_.reset();
}

void PenaltySystem::UpdateEntity(OriGine::EntityHandle _handle) {
    auto* playerState = GetComponent<PlayerState>(_entity);

    // ペナルティ状態でなければ 処理しない
    if (!playerState->IsPenalty()) {
        return;
    };

    // ペナルティ状態を解除
    // マイナスする時間を取得
    float penaltyTime = playerState->SufferPenalty();

    // ギアレベルを下げる
    constexpr int32_t kThresholdPenaltyLevel = 4;
    int32_t newGearLevel                     = playerState->GetGearLevel();
    newGearLevel                             = (std::max)((std::min)(newGearLevel, kThresholdPenaltyLevel), newGearLevel / 2);
    playerState->SetGearLevel(newGearLevel);

    playerState->GetStateFlagRef().CurrentRef().SetFlag(PlayerStateFlag::GEAR_UP);

    // Playerが操作しているエンティティでなければ 処理しない
    if (_entity->IsUnique()) {
        return;
    }

    // TimerComponent を取得 & ペナルティー分 時間をマイナス
    OriGine::Entity* tierEntity = GetUniqueEntity("Timer");
    if (!tierEntity) {
        return;
    }
    TimerComponent* timer = GetComponent<TimerComponent>(tierEntity);
    timer->SetCurrentTime(timer->GetTime() - penaltyTime);

    /// ペナルティー時間を表示する
    int32_t penaltyTimeUIEntityId        = CreateEntity("PenaltyTimeUI");
    OriGine::Entity* penaltyTimeUIEntity = GetEntity(penaltyTimeUIEntityId);
    // Penaltyを表す時間
    TimerComponent penaltyTimer = TimerComponent();
    penaltyTimer.SetMaxTime(penaltyTime);
    penaltyTimer.SetCurrentTime(penaltyTime);

    // 情報だけ持つ事前コンポーネントから情報をコピー
    OriGine::Entity* forSpriteDataEntity     = GetUniqueEntity("PenaltyTimerForSprite");
    TimerForSpriteComponent* forSpriteData   = GetComponent<TimerForSpriteComponent>(forSpriteDataEntity);
    TimerForSpriteComponent timer4SpriteComp = *forSpriteData;
    timer4SpriteComp.SetSpritesEntityId(penaltyTimeUIEntityId);

    // 桁数を設定
    timer4SpriteComp.digitInteger = (std::max)(CountIntegralDigits<float, int>(penaltyTime), timer4SpriteComp.digitInteger);
    timer4SpriteComp.digitDecimal = (std::max)(CountDecimalDigits<float, int>(penaltyTime), timer4SpriteComp.digitDecimal);

    /// コンポーネントを追加
    AddComponent<TimerComponent>(penaltyTimeUIEntity, penaltyTimer, false);
    AddComponent<TimerForSpriteComponent>(penaltyTimeUIEntity, timer4SpriteComp);

    /// スプライトを作成
    createSpriteFromTimerSystem_->CreateSprites(penaltyTimeUIEntity, &timer4SpriteComp);
    // animationを設定
    auto sprites = GetComponents<SpriteRenderer>(penaltyTimeUIEntity);
    if (!sprites) {
        return;
    }

    SpriteAnimation animation = *GetComponent<SpriteAnimation>(forSpriteDataEntity);
    animation.PlayStart();
    int32_t spriteIndex = 0;
    for (auto& sprite : *sprites) {
        // それぞれの位置からアニメーションするように
        for (auto& translate : animation.GetTranslateCurve()) {
            translate.value += sprite.GetTranslate();
        }

        animation.SetSpriteComponentIndex(spriteIndex);
        AddComponent<SpriteAnimation>(penaltyTimeUIEntity, animation);

        ++spriteIndex;
    }

    { /// system に登録
        auto systemRunner = GetScene()->GetSystemRunnerRef();
        systemRunner->RegisterEntity<PenaltyTimeSpriteUpdate>(penaltyTimeUIEntity);
        systemRunner->RegisterEntity<TimerForSprite>(penaltyTimeUIEntity);
        systemRunner->RegisterEntity<SpriteAnimationSystem>(penaltyTimeUIEntity);
    }
}
