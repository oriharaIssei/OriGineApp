#include "CreateSpriteFromTimer.h"

/// engine
#include "Engine.h"
#include "winApp/WinApp.h"

/// ECS
// component
#include "component/renderer/Sprite.h"
#include "component/TimerComponent.h"
// system
#include "system/render/SpriteRenderSystem.h"
#include "system/SystemRunner.h"

CreateSpriteFromTimer::CreateSpriteFromTimer()
    : ISystem(SystemCategory::Initialize) {}
CreateSpriteFromTimer::~CreateSpriteFromTimer() {}

void CreateSpriteFromTimer::Initialize() {}
void CreateSpriteFromTimer::Finalize() {}

void CreateSpriteFromTimer::UpdateEntity(Entity* _entity) {
    auto timerForSpriteComponent = GetComponent<TimerForSpriteComponent>(_entity);
    if (!timerForSpriteComponent) {
        return; // タイマーコンポーネントがない場合は何もしない
    }

    // Sprite用のEntityを作成
    int32_t spriteEntityId = CreateEntity("TimerForSprite_Sprites", false);
    Entity* spriteEntity   = GetEntity(spriteEntityId);
    spriteEntity->SetShouldSave(false); // セーブしない
    CreateSprites(spriteEntity, timerForSpriteComponent);
}

void CreateSpriteFromTimer::CreateSprites(Entity* _entity, TimerForSpriteComponent* _forSpriteComp) {
    // スプライトの生成位置を計算
    Vec2f origin = Engine::GetInstance()->GetWinApp()->GetWindowSize();
    origin *= _forSpriteComp->GetAnchorOnWindow();
    origin += _forSpriteComp->GetOffset();

    int digitInteger = _forSpriteComp->GetDigitIntegerForSprite();
    int digitAll     = _forSpriteComp->GetDigitForSprite();

    Vec2f uvScale = _forSpriteComp->GetNumberTileSize() / _forSpriteComp->GetNumbersTextureSize();

    Vec2f pos = origin;

    // 整数部の合計幅
    float widthInteger = digitInteger * _forSpriteComp->GetSpriteSizeInteger()[X]
                         + (digitInteger > 0 ? (digitInteger - 1) * _forSpriteComp->GetSpriteMarginInteger()[X] : 0);

    // 区切りスペース
    float widthBetween = _forSpriteComp->GetMarginBetweenIntegerAndDecimal()[X];

    // 「整数部の右端」と「小数部の左端」の間（区切りスペースの中心）がoriginになるように調整
    pos[X] -= widthInteger;
    pos[X] += widthBetween * 0.5f;

    // Sprite用のEntityを作成
    GetScene()->GetSystemRunnerRef()->GetSystemRef<SpriteRenderSystem>()->AddEntity(_entity);

    _forSpriteComp->SetSpritesEntityId(_entity->GetID());

    // 整数部
    for (int i = 0; i < digitInteger; ++i) {
        AddComponent<SpriteRenderer>(_entity, SpriteRenderer{}, true);
        auto* sprite = GetComponent<SpriteRenderer>(_entity, i);

        sprite->SetIsRender(true);

        sprite->SetTexture(_forSpriteComp->GetNumbersTexturePath(), false);
        sprite->SetAnchorPoint({0.5f, 0.5f});
        sprite->SetSize(_forSpriteComp->GetSpriteSizeInteger());
        sprite->SetTextureSize(_forSpriteComp->GetNumbersTextureSize());
        sprite->SetUVScale(uvScale);
        sprite->SetTranslate(pos);

        pos[X] += _forSpriteComp->GetSpriteSizeInteger()[X];
        pos += _forSpriteComp->GetSpriteMarginInteger();
    }

    pos += _forSpriteComp->GetMarginBetweenIntegerAndDecimal(); // 整数部と小数部の間のスペース

    // 小数部
    for (int i = digitInteger; i < digitAll; ++i) {
        AddComponent<SpriteRenderer>(_entity, SpriteRenderer{}, true);
        auto* sprite = GetComponent<SpriteRenderer>(_entity, i);

        sprite->SetTexture(_forSpriteComp->GetNumbersTexturePath(), false);
        sprite->SetAnchorPoint({0.5f, 0.5f});
        sprite->SetSize(_forSpriteComp->GetSpriteSizeDecimal());
        sprite->SetTextureSize(_forSpriteComp->GetNumbersTextureSize());
        sprite->SetUVScale(uvScale);
        sprite->SetTranslate(pos);

        pos[X] += _forSpriteComp->GetSpriteSizeDecimal()[X];
        pos += _forSpriteComp->GetSpriteMarginDecimal();
    }
}
