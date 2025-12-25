#include "StartTimerInitialize.h"

/// engine
#include "scene/Scene.h"

/// ECS
// component
#include "component/animation/SpriteAnimation.h"
#include "component/renderer/Sprite.h"
#include "component/TimerComponent.h"

// system
#include "system/effect/SpriteAnimationSystem.h"

using namespace OriGine;

StartTimerInitialize::StartTimerInitialize() : ISystem(OriGine::SystemCategory::Initialize) {}
StartTimerInitialize::~StartTimerInitialize() {}

void StartTimerInitialize::Initialize() {
    createSpriteFromTimerSystem_ = std::make_unique<CreateSpriteFromTimer>();
    createSpriteFromTimerSystem_->SetScene(GetScene());
    createSpriteFromTimerSystem_->Initialize();
}
void StartTimerInitialize::Finalize() {}

void StartTimerInitialize::UpdateEntity(OriGine::EntityHandle _handle) {
    auto timerComp    = GetComponent<TimerComponent>(_handle);
    auto timer4Sprite = GetComponent<TimerForSpriteComponent>(_handle);
    if (!timerComp || !timer4Sprite) {
        return;
    }
    timerComp->SetStarted(true);

    // sprite の生成
    OriGine::Entity* spriteEntity = GetEntity(timer4Sprite->GetSpritesEntityHandle());
    if (!spriteEntity) {
        return;
    }

    auto& sprites = GetComponents<SpriteRenderer>(spriteEntity->GetHandle());
    if (sprites.empty()) {
        return;
    }

    auto* originalAnimation = GetComponent<SpriteAnimation>(_handle);
    int32_t spriteCompIndex = 0;
    for (auto& sprite : sprites) {
        // 生成
        ComponentHandle spriteAnimationHandle = AddComponent<SpriteAnimation>(spriteEntity->GetHandle());
        // 取得
        SpriteAnimation* spriteAnimation = GetComponent<SpriteAnimation>(spriteAnimationHandle);
        // コピー
        *spriteAnimation = *originalAnimation;
        // 再生開始
        spriteAnimation->PlayColorAnimation();
        spriteAnimation->PlayTransformAnimation();
        spriteAnimation->SetSpriteComponentIndex(spriteCompIndex);

        sprite.SetIsRender(true);

        ++spriteCompIndex;
    }
    GetScene()->GetSystem(nameof<SpriteAnimationSystem>())->AddEntity(spriteEntity->GetHandle());
}
