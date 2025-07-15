#include "TimerForSprite.h"

/// ECS
#include "ECSManager.h"
// component
#include "component/renderer/Sprite.h"
#include "component/TimerComponent.h"

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
    : ISystem(SystemType::Effect) {}

TimerForSprite::~TimerForSprite() {}

void TimerForSprite::Initialize() {}

void TimerForSprite::Finalize() {}

void TimerForSprite::UpdateEntity(GameEntity* _entity) {
    auto timerComponent          = getComponent<TimerComponent>(_entity);
    auto timerForSpriteComponent = getComponent<TimerForSpriteComponent>(_entity);
    if (!timerComponent) {
        return; // タイマーコンポーネントがない場合は何もしない
    }

    std::vector<int> digits = CalculateDigitsFromFloat(
        timerComponent->getCurrentTime(),
        timerForSpriteComponent->getDigitIntegerForSprite(),
        timerForSpriteComponent->getDigitDecimalForSprite());

    for (int32_t i = 0; i < timerForSpriteComponent->getDigitForSprite(); ++i) {
        auto sprite = ECSManager::getInstance()->getComponent<SpriteRenderer>(_entity, i);
        if (!sprite) {
            continue; // スプライトがない場合は何もしない
        }
        float spriteLeftTopY     = sprite->getTextureLeftTop()[Y];
        float spriteTextureSizeX = sprite->getTextureSize()[X];

        sprite->setTextureLeftTop(
            {spriteTextureSizeX * digits[i], spriteLeftTopY});
    }
}
