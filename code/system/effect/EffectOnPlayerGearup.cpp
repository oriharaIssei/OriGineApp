#include "EffectOnPlayerGearup.h"

/// Engine
#define ENGINE_ECS
#define DELTA_TIME
#include "EngineInclude.h"

// component
#include "component/effect/particle/emitter/Emitter.h"
#include "component/effect/post/DistortionEffectParam.h"
#include "component/effect/post/RadialBlurParam.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"
#include "component/renderer/primitive/RingRenderer.h"

/// math
#include <math/MyEasing.h>

EffectOnPlayerGearup::EffectOnPlayerGearup() : ISystem(SystemCategory::Effect) {}

EffectOnPlayerGearup::~EffectOnPlayerGearup() {}

void EffectOnPlayerGearup::Initialize() {}

void EffectOnPlayerGearup::Finalize() {}

void EffectOnPlayerGearup::UpdateEntity(Entity* _entity) {
    Entity* player = getUniqueEntity("Player");
    if (!player) {
        return;
    }
    PlayerState* state         = getComponent<PlayerState>(player);
    Transform* playerTransform = getComponent<Transform>(player);

    if (!state) {
        return;
    }

    /// ==============================
    // Effect PlayState Transition
    /// ==============================
    shockWaveState_.playState_.set(false);
    if (state->isGearUp()) {

        Transform* transform = getComponent<Transform>(_entity);
        transform->translate = playerTransform->translate; // Playerの位置に合わせる

        // Emitterの再生
        int32_t emitterSize = getComponentArray<Emitter>()->getComponentSize(_entity);
        for (int32_t i = 0; i < emitterSize; ++i) {
            Emitter* emitter = getComponent<Emitter>(_entity, i);
            if (emitter) {
                emitter->setOriginPos(emitterOffset_);
                emitter->PlayStart();
            }
        }

        if (state->getGearLevel() >= 2) {
            shockWaveState_.playState_.set(true);
        }
    }

    UpdateShockWaveRing(_entity, playerTransform);
}

void EffectOnPlayerGearup::UpdateShockWaveRing(Entity* _entity, Transform* _playerTransform) {
    DistortionEffectParam* distortionEffectParam = getComponent<DistortionEffectParam>(_entity);

    shockWaveState_.currentTime += getMainDeltaTime();
    float t = shockWaveState_.currentTime / shockWaveState_.maxTime;

    if (t >= 1.f) {
        t = 1.f;
        shockWaveState_.playState_.set(false);
    }

    shockWaveState_.playState_.sync();

    auto& shockWaveRings = distortionEffectParam->getDistortionObjects();
    if (!shockWaveState_.playState_.current()) {
        if (shockWaveState_.playState_.isRelease()) {
            // shockWaveRingのを見えない位置に隠す
            for (auto& [object, type] : shockWaveRings) {
                if (type != PrimitiveType::Ring) {
                    continue;
                }
                if (object == nullptr) {
                    continue;
                }

                RingRenderer* ringRenderer = dynamic_cast<RingRenderer*>(object.get());

                // 初期位置
                ringRenderer->getTransform().translate[Y] = -10000.f;
            }
        }

        return;
    }

    if (shockWaveState_.playState_.isTrigger()) {

        // shockWaveRingの初期化Add commentMore actions
        for (auto& [object, type] : shockWaveRings) {
            if (type != PrimitiveType::Ring) {
                continue;
            }
            if (object == nullptr) {
                continue;
            }

            RingRenderer* ringRenderer          = dynamic_cast<RingRenderer*>(object.get());
            ringRenderer->getTransform().parent = _playerTransform; // PlayerのTransformを親に設定

            ringRenderer->getTransform().translate = shockWaveOffset_; // 初期位置を設定
            ringRenderer->getTransform().rotate    = _playerTransform->rotate;
            ringRenderer->getPrimitive().setInnerRadius(minInnerRadius_);
            ringRenderer->getPrimitive().setOuterRadius(minOuterRadius_);
        }

        shockWaveState_.currentTime = 0.f; // 時間をリセット
    }

    /// shockWave Play
    if (shockWaveState_.playState_.current()) {
        float easeT = EaseInQuad(t);
        // 衝撃波の拡大
        for (auto& [object, type] : shockWaveRings) {
            if (type != PrimitiveType::Ring) {
                continue;
            }
            if (object == nullptr) {
                continue;
            }
            RingRenderer* ringRenderer = dynamic_cast<RingRenderer*>(object.get());

            float innerRadius = std::lerp(minInnerRadius_, maxInnerRadius_, easeT);
            float outerRadius = std::lerp(minOuterRadius_, maxOuterRadius_, easeT);
            ringRenderer->getPrimitive().setInnerRadius(innerRadius);
            ringRenderer->getPrimitive().setOuterRadius(outerRadius);
            ringRenderer->createMesh(&ringRenderer->getMeshGroup()->back());
        }
    };
}
