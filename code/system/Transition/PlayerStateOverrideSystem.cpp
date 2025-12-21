#include "PlayerStateOverrideSystem.h"

/// engine
#include "Engine.h"
#include "scene/Scene.h"
// input
#include "input/GamepadInput.h"

/// ECS
// component
#include "component/collision/CollisionPushBackInfo.h"
#include "component/PlayerStateOverrideCondition.h"
#include "component/renderer/Sprite.h"

using namespace OriGine;

PlayerStateOverrideSystem::PlayerStateOverrideSystem() : ISystem(SystemCategory::StateTransition) {}
PlayerStateOverrideSystem::~PlayerStateOverrideSystem() {}

void PlayerStateOverrideSystem::Initialize() {}
void PlayerStateOverrideSystem::Finalize() {}

void PlayerStateOverrideSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    auto pushBackInfo      = GetComponent<CollisionPushBackInfo>(_entity);
    auto overrideCondition = GetComponent<PlayerStateOverrideCondition>(_entity);

    auto keyUi     = GetComponent<SpriteRenderer>(_entity, 0);
    auto gamepadUi = GetComponent<SpriteRenderer>(_entity, 1);
    if (!keyUi || !gamepadUi) {
        return;
    }
    // UIを非表示にする
    keyUi->SetIsRender(false);
    gamepadUi->SetIsRender(false);

    if (!pushBackInfo || !overrideCondition) {
        return;
    }

    for (auto& [id, info] : pushBackInfo->GetCollisionInfoMap()) {
        Entity* otherEnt = GetEntity(id);
        if (!otherEnt) {
            continue;
        }

        auto playerState = GetComponent<PlayerState>(otherEnt);
        if (!playerState) {
            continue;
        }

        // 状態が一致しなければ、時間を止める
        if (!overrideCondition->overrideConditions.HasFlag(playerState->GetStateEnum())) {
            constexpr float kTimeRate = 0.1f;
            Engine* engine            = Engine::GetInstance();
            engine->SetDeltaTime(engine->GetDeltaTime() * kTimeRate);

            // 対応した spriteを表示
            if (GetScene()->GetGamepadInput()->IsActive()) {
                keyUi->SetIsRender(false);
                gamepadUi->SetIsRender(true);
            } else {
                keyUi->SetIsRender(true);
                gamepadUi->SetIsRender(false);
            }

            return;
        }
    }
}
