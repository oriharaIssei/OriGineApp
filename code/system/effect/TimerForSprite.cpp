#include "TimerForSprite.h"

/// engine
#include "Engine.h"
#include "winApp/WinApp.h"

/// ECS
// component
#include "component/renderer/Sprite.h"
#include "component/TimerComponent.h"

/// math
#include "math/mathEnv.h"

TimerForSprite::TimerForSprite()
    : ISystem(SystemCategory::Effect) {}

TimerForSprite::~TimerForSprite() {}

void TimerForSprite::Initialize() {}

void TimerForSprite::Finalize() {}

void TimerForSprite::UpdateEntity(Entity* _entity) {
    auto timerComponent          = GetComponent<TimerComponent>(_entity);
    auto timerForSpriteComponent = GetComponent<TimerForSpriteComponent>(_entity);
    if (!timerComponent || !timerForSpriteComponent) {
        return; // タイマーコンポーネントがない場合は何もしない
    }

    auto timerSpritesEntity = GetEntity(timerForSpriteComponent->GetSpritesEntityId());
    if (!timerSpritesEntity) {
        return; // スプライトエンティティがない場合は何もしない
    }

    // 浮動小数点数から各桁の数字を抽出
    std::vector<int> digits = CalculateDigitsFromFloat(
        timerComponent->GetTime(),
        timerForSpriteComponent->digitInteger,
        timerForSpriteComponent->digitDecimal);

    Vec2f windowSize = Engine::GetInstance()->GetWinApp()->GetWindowSize();

    // 各スプライトに数字を適用
    for (int32_t i = 0; i < timerForSpriteComponent->digitInteger + timerForSpriteComponent->digitDecimal; ++i) {
        auto sprite = GetComponent<SpriteRenderer>(timerSpritesEntity, i);
        if (!sprite) {
            continue; // スプライトがない場合は何もしない
        }
        if (int32_t(digits.size()) <= i) {
            return;
        }
        float spriteLeftTopY     = sprite->GetTextureLeftTop()[Y];
        float spriteTextureSizeX = sprite->GetTextureSize()[X];

        sprite->SetTextureLeftTop(
            {spriteTextureSizeX * digits[i], spriteLeftTopY});

        sprite->CalculatePosRatioAndSizeRatio(windowSize);
    }
}
