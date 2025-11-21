#include "CreateSpriteFromTimer.h"

/// engine
#include "Engine.h"
#include "winApp/WinApp.h"

/// ECS
// component
#include "component/renderer/Sprite.h"
#include "component/TimerComponent.h"
// system
#include "system/SystemRunner.h"
#include "system/render/SpriteRenderSystem.h"

CreateSpriteFromTimer::CreateSpriteFromTimer()
    : ISystem(SystemCategory::Initialize) {}

CreateSpriteFromTimer::~CreateSpriteFromTimer() {}

void CreateSpriteFromTimer::Initialize() {}

void CreateSpriteFromTimer::Finalize() {}

void CreateSpriteFromTimer::UpdateEntity(Entity* _entity) {
    auto timerComponent          = GetComponent<TimerComponent>(_entity);
    auto timerForSpriteComponent = GetComponent<TimerForSpriteComponent>(_entity);
    if (!timerComponent || !timerForSpriteComponent) {
        return; // タイマーコンポーネントがない場合は何もしない
    }
    Vec2f origin = Engine::GetInstance()->GetWinApp()->GetWindowSize();
    origin *= timerForSpriteComponent->GetAnchorOnWindow();
    origin += timerForSpriteComponent->GetOffset();

    int digitInteger = timerForSpriteComponent->GetDigitIntegerForSprite();
    int digitAll     = timerForSpriteComponent->GetDigitForSprite();

    Vec2f uvScale = timerForSpriteComponent->GetNumberTileSize() / timerForSpriteComponent->GetNumbersTextureSize();

    Vec2f pos = origin;

    // 整数部の合計幅
    float widthInteger = digitInteger * timerForSpriteComponent->GetSpriteSizeInteger()[X]
                         + (digitInteger > 0 ? (digitInteger - 1) * timerForSpriteComponent->GetSpriteMarginInteger()[X] : 0);

    // 区切りスペース
    float widthBetween = timerForSpriteComponent->GetMarginBetweenIntegerAndDecimal()[X];

    // 「整数部の右端」と「小数部の左端」の間（区切りスペースの中心）がoriginになるように調整
    pos[X] -= widthInteger;
    pos[X] += widthBetween * 0.5f;

    // Sprite用のEntityを作成
    int32_t spriteEntityId   = CreateEntity("TimerForSprite_Sprites", false);
    Entity* spriteEntity = GetEntity(spriteEntityId);
    spriteEntity->SetShouldSave(false); // セーブしない

    GetScene()->GetSystemRunnerRef()->GetSystemRef<SpriteRenderSystem>()->AddEntity(spriteEntity);

    timerForSpriteComponent->SetSpritesEntityId(spriteEntityId);

    // 整数部
    for (int i = 0; i < digitInteger; ++i) {
        AddComponent<SpriteRenderer>(spriteEntity, SpriteRenderer{}, true);
        auto* sprite = GetComponent<SpriteRenderer>(spriteEntity, i);

        sprite->SetIsRender(true);

        sprite->SetTexture(timerForSpriteComponent->GetNumbersTexturePath(), false);
        sprite->SetAnchorPoint({0.5f, 0.5f});
        sprite->SetSize(timerForSpriteComponent->GetSpriteSizeInteger());
        sprite->SetTextureSize(timerForSpriteComponent->GetNumbersTextureSize());
        sprite->SetUVScale(uvScale);
        sprite->SetTranslate(pos);

        pos[X] += timerForSpriteComponent->GetSpriteSizeInteger()[X];
        pos += timerForSpriteComponent->GetSpriteMarginInteger();
    }

    pos += timerForSpriteComponent->GetMarginBetweenIntegerAndDecimal(); // 整数部と小数部の間のスペース

    // 小数部
    for (int i = digitInteger; i < digitAll; ++i) {
        AddComponent<SpriteRenderer>(spriteEntity, SpriteRenderer{}, true);
        auto* sprite = GetComponent<SpriteRenderer>(spriteEntity, i);

        sprite->SetTexture(timerForSpriteComponent->GetNumbersTexturePath(), false);
        sprite->SetAnchorPoint({0.5f, 0.5f});
        sprite->SetSize(timerForSpriteComponent->GetSpriteSizeDecimal());
        sprite->SetTextureSize(timerForSpriteComponent->GetNumbersTextureSize());
        sprite->SetUVScale(uvScale);
        sprite->SetTranslate(pos);

        pos[X] += timerForSpriteComponent->GetSpriteSizeDecimal()[X];
        pos += timerForSpriteComponent->GetSpriteMarginDecimal();
    }
}
