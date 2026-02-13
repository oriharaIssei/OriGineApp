#include "TimerForSprite.h"

/// engine
#include "Engine.h"
#include "winApp/WinApp.h"

/// ECS
// component
#include "component/renderer/Sprite.h"
#include "component/TimerComponent.h"
#include "component/TimerForSpriteComponent.h"

/// math
#include "math/mathEnv.h"

using namespace OriGine;

TimerForSprite::TimerForSprite()
    : ISystem(OriGine::SystemCategory::Effect) {}
TimerForSprite::~TimerForSprite() {}

void TimerForSprite::Initialize() {}
void TimerForSprite::Finalize() {}

void TimerForSprite::UpdateEntity(OriGine::EntityHandle _handle) {
    auto timerComponent          = GetComponent<TimerComponent>(_handle);
    auto timerForSpriteComponent = GetComponent<TimerForSpriteComponent>(_handle);
    if (!timerComponent || !timerForSpriteComponent) {
        return; // タイマーコンポーネントがない場合は何もしない
    }

    auto timerSpritesEntity = GetEntity(timerForSpriteComponent->GetSpritesEntityHandle());
    if (!timerSpritesEntity) {
        return; // スプライトエンティティがない場合は何もしない
    }

    // 浮動小数点数から各桁の数字を抽出
    std::vector<int> digits = CalculateDigitsFromFloat(
        timerComponent->GetTime(),
        timerForSpriteComponent->digitInteger,
        timerForSpriteComponent->digitDecimal);

    OriGine::Vec2f windowSize = Engine::GetInstance()->GetWinApp()->GetWindowSize();

    // 各スプライトに数字を適用
    for (int32_t i = 0; i < timerForSpriteComponent->digitInteger + timerForSpriteComponent->digitDecimal; ++i) {
        if (int32_t(digits.size()) <= i) {
            return;
        }

        auto sprite = GetComponent<SpriteRenderer>(timerSpritesEntity->GetHandle(), i);
        if (!sprite) {
            continue; // スプライトがない場合は何もしない
        }
        float spriteLeftTopY     = sprite->GetTextureLeftTop()[Y];
        float spriteTextureSizeX = sprite->GetTextureSize()[X];

        sprite->SetTextureLeftTop(
            {spriteTextureSizeX * digits[i], spriteLeftTopY});

        sprite->CalculatePosRatioAndSizeRatio(windowSize);
    }
}
