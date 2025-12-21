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
    auto timerComp    = GetComponent<TimerComponent>(_entity);
    auto timer4Sprite = GetComponent<TimerForSpriteComponent>(_entity);
    if (!timerComp || !timer4Sprite) {
        return;
    }
    timerComp->SetStarted(true);

    // sprite の生成

    OriGine::Entity* spriteEntity = GetEntity(timer4Sprite->GetSpritesEntityId());
    if (!spriteEntity) {
        return;
    }

    auto* sprites = GetComponents<SpriteRenderer>(spriteEntity);
    if (sprites) {
        auto* originalAnimation = GetComponent<SpriteAnimation>(_entity);
        int32_t spriteCompIndex = 0;
        for (auto& sprite : *sprites) {
            SpriteAnimation copiedAnimation = *originalAnimation;
            copiedAnimation.PlayColorAnimation();
            copiedAnimation.PlayTransformAnimation();
            copiedAnimation.SetSpriteComponentIndex(spriteCompIndex);
            AddComponent<SpriteAnimation>(spriteEntity, copiedAnimation);

            sprite.SetIsRender(true);

            ++spriteCompIndex;
        }
        GetScene()->GetSystem(nameof<SpriteAnimationSystem>())->AddEntity(spriteEntity);
    }
}
