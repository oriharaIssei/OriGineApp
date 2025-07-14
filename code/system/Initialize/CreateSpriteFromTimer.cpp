#include "CreateSpriteFromTimer.h"

/// engine
#include "Engine.h"
#include "winApp/WinApp.h"
/// ECS

// component
#include "component/renderer/Sprite.h"
#include "component/TimerComponent.h"

CreateSpriteFromTimer::CreateSpriteFromTimer()
    : ISystem(SystemCategory::Initialize) {}

CreateSpriteFromTimer::~CreateSpriteFromTimer() {}

void CreateSpriteFromTimer::Initialize() {}

void CreateSpriteFromTimer::Finalize() {}

void CreateSpriteFromTimer::UpdateEntity(GameEntity* _entity) {
    auto timerComponent          = getComponent<TimerComponent>(_entity);
    auto timerForSpriteComponent = getComponent<TimerForSpriteComponent>(_entity);
    if (!timerComponent || !timerForSpriteComponent) {
        return; // タイマーコンポーネントがない場合は何もしない
    }
    Vec2f origin = Engine::getInstance()->getWinApp()->getWindowSize();
    origin *= timerForSpriteComponent->getAnchorOnWindow();
    origin += timerForSpriteComponent->getOffset();

    int digitInteger = timerForSpriteComponent->getDigitIntegerForSprite();
    int digitAll     = timerForSpriteComponent->getDigitForSprite();

    Vec2f uvScale = timerForSpriteComponent->getNumberTileSize() / timerForSpriteComponent->getNumbersTextureSize();

    Vec2f pos = origin;

    // 整数部の合計幅
    float widthInteger = digitInteger * timerForSpriteComponent->getSpriteSizeInteger()[X]
                         + (digitInteger > 0 ? (digitInteger - 1) * timerForSpriteComponent->getSpriteMarginInteger()[X] : 0);

    // 区切りスペース
    float widthBetween = timerForSpriteComponent->getMarginBetweenIntegerAndDecimal()[X];

    // 「整数部の右端」と「小数部の左端」の間（区切りスペースの中心）がoriginになるように調整
    pos[X] -= widthInteger;
    pos[X] += widthBetween * 0.5f;

    // 整数部
    for (int i = 0; i < digitInteger; ++i) {
        ECSManager::getInstance()->addComponent<SpriteRenderer>(_entity->getID(), SpriteRenderer{}, true);
        auto* sprite = getComponent<SpriteRenderer>(_entity, i);

        sprite->setTexture(timerForSpriteComponent->getNumbersTexturePath(), false);
        sprite->setAnchorPoint({0.5f, 0.5f});
        sprite->setSize(timerForSpriteComponent->getSpriteSizeInteger());
        sprite->setTextureSize(timerForSpriteComponent->getNumbersTextureSize());
        sprite->setUVScale(uvScale);
        sprite->setTranslate(pos);

        pos[X] += timerForSpriteComponent->getSpriteSizeInteger()[X];
        pos += timerForSpriteComponent->getSpriteMarginInteger();
    }

    pos += timerForSpriteComponent->getMarginBetweenIntegerAndDecimal(); // 整数部と小数部の間のスペース

    // 小数部
    for (int i = digitInteger; i < digitAll; ++i) {
        ECSManager::getInstance()->addComponent<SpriteRenderer>(_entity->getID(), SpriteRenderer{}, true);
        auto* sprite = getComponent<SpriteRenderer>(_entity, i);

        sprite->setTexture(timerForSpriteComponent->getNumbersTexturePath(), false);
        sprite->setAnchorPoint({0.5f, 0.5f});
        sprite->setSize(timerForSpriteComponent->getSpriteSizeDecimal());
        sprite->setTextureSize(timerForSpriteComponent->getNumbersTextureSize());
        sprite->setUVScale(uvScale);
        sprite->setTranslate(pos);

        pos[X] += timerForSpriteComponent->getSpriteSizeDecimal()[X];
        pos += timerForSpriteComponent->getSpriteMarginDecimal();
    }
}
