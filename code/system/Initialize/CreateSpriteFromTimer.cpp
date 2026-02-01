#include "CreateSpriteFromTimer.h"

/// engine
#include "Engine.h"
#include "scene/Scene.h"
#include "winApp/WinApp.h"

/// ECS
// component
#include "component/renderer/Sprite.h"
#include "component/TimerComponent.h"
// system
#include "system/render/SpriteRenderSystem.h"
#include "system/SystemRunner.h"

using namespace OriGine;

CreateSpriteFromTimer::CreateSpriteFromTimer()
    : ISystem(OriGine::SystemCategory::Initialize) {}
CreateSpriteFromTimer::~CreateSpriteFromTimer() {}

void CreateSpriteFromTimer::Initialize() {}
void CreateSpriteFromTimer::Finalize() {}

void CreateSpriteFromTimer::UpdateEntity(OriGine::EntityHandle _handle) {
    auto timerForSpriteComponent = GetComponent<TimerForSpriteComponent>(_handle);
    if (!timerForSpriteComponent) {
        return; // タイマーコンポーネントがない場合は何もしない
    }

    // Sprite用のEntityを作成
    EntityHandle spriteEntityId   = CreateEntity("TimerForSprite_Sprites", false);
    OriGine::Entity* spriteEntity = GetEntity(spriteEntityId);
    spriteEntity->SetShouldSave(false); // セーブしない
    CreateSprites(spriteEntityId, timerForSpriteComponent);
}

void CreateSpriteFromTimer::CreateSprites(OriGine::EntityHandle _handle, TimerForSpriteComponent* _forSpriteComp) {
    // スプライトの生成位置を計算
    OriGine::Vec2f origin = OriGine::Engine::GetInstance()->GetWinApp()->GetWindowSize() * _forSpriteComp->anchorOnWindow;
    origin += _forSpriteComp->offset;

    int digitInteger = _forSpriteComp->digitInteger;
    int digitAll     = digitInteger + _forSpriteComp->digitDecimal;

    OriGine::Vec2f uvScale = _forSpriteComp->numberTileSize / _forSpriteComp->numbersTextureSize;

    OriGine::Vec2f pos = origin;

    // 整数部の合計幅
    float widthInteger = digitInteger * _forSpriteComp->spriteSizeInteger[X]
                         + (digitInteger > 0 ? (digitInteger - 1) * _forSpriteComp->spriteMarginInteger[X] : 0);

    // 区切りスペース
    float widthBetween = _forSpriteComp->marginBetweenIntegerAndDecimal[X];

    // 「整数部の右端」と「小数部の左端」の間（区切りスペースの中心）がoriginになるように調整
    pos[X] -= widthInteger;
    pos[X] += widthBetween * 0.5f;

    // Sprite用のEntityを作成
    GetScene()->GetSystemRunnerRef()->GetSystemRef<SpriteRenderSystem>()->AddEntity(_handle);

    _forSpriteComp->SetSpritesEntityHandle(_handle);

    // 整数部
    for (int i = 0; i < digitInteger; ++i) {
        auto* sprite = GetComponent<SpriteRenderer>(AddComponent<SpriteRenderer>(_handle));
        if (sprite) {
            sprite->SetIsRender(true);

            sprite->SetTexture(_forSpriteComp->numbersTexturePath, false);
            sprite->SetAnchorPoint({0.5f, 0.5f});
            sprite->SetSize(_forSpriteComp->spriteSizeInteger);
            sprite->SetTextureSize(_forSpriteComp->numbersTextureSize);
            sprite->SetUVScale(uvScale);
            sprite->SetTranslate(pos);

            pos[X] += _forSpriteComp->spriteSizeInteger[X];
            pos += _forSpriteComp->spriteMarginInteger;
        }
    }

    // 整数部と小数部の間のスペース
    pos += _forSpriteComp->marginBetweenIntegerAndDecimal;

    // 小数部
    for (int i = digitInteger; i < digitAll; ++i) {
        auto* sprite = GetComponent<SpriteRenderer>(AddComponent<SpriteRenderer>(_handle));
        if (sprite) {
            sprite->SetIsRender(true);

            sprite->SetTexture(_forSpriteComp->numbersTexturePath, false);
            sprite->SetAnchorPoint({0.5f, 0.5f});
            sprite->SetSize(_forSpriteComp->spriteSizeDecimal);
            sprite->SetTextureSize(_forSpriteComp->numbersTextureSize);
            sprite->SetUVScale(uvScale);
            sprite->SetTranslate(pos);

            pos[X] += _forSpriteComp->spriteSizeDecimal[X];
            pos += _forSpriteComp->spriteMarginDecimal;
        }
    }
}
