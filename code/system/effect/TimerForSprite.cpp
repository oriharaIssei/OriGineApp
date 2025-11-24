#include "TimerForSprite.h"

/// ECS

// component
#include "component/renderer/Sprite.h"
#include "component/TimerComponent.h"

/// <summary>
/// 浮動小数点数から整数部と小数部の各桁の数字を抽出する
/// </summary>
/// <param name="value">変換前</param>
/// <param name="intDigits">整数部の桁数</param>
/// <param name="fracDigits">小数部の桁数</param>
/// <returns></returns>
static std::vector<int> CalculateDigitsFromFloat(float value, int intDigits, int fracDigits) {
    std::vector<int> digits;

    value = std::fabs(value);

    // 整数部の抽出
    int intPart = static_cast<int>(value);
    for (int i = intDigits - 1; i >= 0; --i) {
        int divisor = static_cast<int>(std::pow(10, i));
        int digit   = intPart / divisor;
        digits.push_back(digit);
        intPart %= divisor;
    }

    // 小数部の抽出
    float fracPart = value - static_cast<int>(value);
    for (int i = 0; i < fracDigits; ++i) {
        fracPart *= 10.0f;
        int digit = static_cast<int>(fracPart);
        digits.push_back(digit);
        fracPart -= digit;
    }

    return digits;
}

TimerForSprite::TimerForSprite()
    : ISystem(SystemCategory::Effect) {}

TimerForSprite::~TimerForSprite() {}

void TimerForSprite::Initialize() {}

void TimerForSprite::Finalize() {}

void TimerForSprite::UpdateEntity(Entity* _entity) {
    auto timerComponent          = GetComponent<TimerComponent>(_entity);
    auto timerForSpriteComponent = GetComponent<TimerForSpriteComponent>(_entity);
    if (!timerComponent) {
        return; // タイマーコンポーネントがない場合は何もしない
    }

    auto timerSpritesEntity = GetEntity(timerForSpriteComponent->GetSpritesEntityId());
    if (!timerSpritesEntity) {
        return; // スプライトエンティティがない場合は何もしない
    }

    // 浮動小数点数から各桁の数字を抽出
    std::vector<int> digits = CalculateDigitsFromFloat(
        timerComponent->GetTime(),
        timerForSpriteComponent->GetDigitIntegerForSprite(),
        timerForSpriteComponent->GetDigitDecimalForSprite());

    // 各スプライトに数字を適用
    for (int32_t i = 0; i < timerForSpriteComponent->GetDigitForSprite(); ++i) {
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
    }
}
