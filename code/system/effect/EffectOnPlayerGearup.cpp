#include "EffectOnPlayerGearup.h"

/// Engine
#define ENGINE_ECS
#define DELTA_TIME
#include "EngineInclude.h"

// component
#include "component/material/Material.h"

#include "component/renderer/primitive/RingRenderer.h"

#include "component/effect/particle/emitter/Emitter.h"
#include "component/effect/post/DistortionEffectParam.h"

#include "component/player/PlayerEffectControlParam.h"
#include "component/player/PlayerStatus.h"
#include "component/player/state/PlayerState.h"

/// math
#include <math/MyEasing.h>

EffectOnPlayerGearup::EffectOnPlayerGearup() : ISystem(SystemCategory::Effect) {}

EffectOnPlayerGearup::~EffectOnPlayerGearup() {}

void EffectOnPlayerGearup::Initialize() {}

void EffectOnPlayerGearup::Finalize() {}

void EffectOnPlayerGearup::UpdateEntity(Entity* _entity) {
    Entity* player = GetUniqueEntity("Player");
    if (!player) {
        return;
    }
    PlayerState* state         = GetComponent<PlayerState>(player);
    Transform* playerTransform = GetComponent<Transform>(player);

    if (!state) {
        return;
    }

    /// ==============================
    // Effect PlayState Transition
    /// ==============================

    shockWaveState_.playState.Sync();
    if (state->IsGearUp()) {
        shockWaveState_.playState.SetCurrent(false);

        Transform* transform = GetComponent<Transform>(_entity);
        transform->translate = playerTransform->translate; // Playerの位置に合わせる

        // Emitterの再生
        int32_t emitterSize = GetComponentArray<Emitter>()->GetComponentSize(_entity);
        for (int32_t i = 0; i < emitterSize; ++i) {
            Emitter* emitter = GetComponent<Emitter>(_entity, i);
            if (emitter) {
                emitter->SetOriginPos(emitterOffset_);
                emitter->PlayStart();
            }
        }

        // trailの色をGearLevelに応じて変化
        PlayerEffectControlParam* effectControlParam = GetComponent<PlayerEffectControlParam>(player);
        Vector4f trailColor                          = effectControlParam->GetTrailColorByGearLevel(state->GetGearLevel());
        if (effectControlParam) {
            Material* material = GetComponent<Material>(GetUniqueEntity("Trail"));
            if (material) {
                material->color_ = trailColor;
                material->UpdateUvMatrix();
            }
        }

        // BackFire の色をGearLevelに応じて変化
        Entity* backFireEntity = GetUniqueEntity("BackFire");
        if (backFireEntity) {
            Material* material = GetComponent<Material>(backFireEntity);
            material->color_   = trailColor;
        }
        Entity* backFireSparksEntity = GetUniqueEntity("BackFireSparks");
        if (backFireSparksEntity) {
            Material* material = GetComponent<Material>(backFireSparksEntity);
            material->color_   = trailColor;
        }

        // GearLevelに応じて衝撃波を発生
        if (state->GetGearLevel() >= 2) {
            shockWaveState_.playState.SetCurrent(true);
            shockWaveState_.currentTime = 0.f;
        }
    }

    UpdateShockWaveRing(_entity, playerTransform);
}

void EffectOnPlayerGearup::UpdateShockWaveRing(Entity* _entity, Transform* _playerTransform) {
    DistortionEffectParam* distortionEffectParam = GetComponent<DistortionEffectParam>(_entity);

    auto& shockWaveRings = distortionEffectParam->GetDistortionObjects();
    // 更新時 以外
    if (shockWaveState_.playState.Current()) { /// shockWave Play

        // 初期化処理
        if (shockWaveState_.playState.IsTrigger()) {

            // shockWaveRingの初期化Add commentMore actions
            for (auto& [object, type] : shockWaveRings) {
                if (type != PrimitiveType::Ring) {
                    continue;
                }
                if (object == nullptr) {
                    continue;
                }

                RingRenderer* ringRenderer = dynamic_cast<RingRenderer*>(object.get());

                ringRenderer->GetTransform().translate    = _playerTransform->translate + shockWaveOffset_; // 初期位置を設定
                ringRenderer->GetTransform().rotate       = _playerTransform->rotate;
                ringRenderer->GetPrimitive().innerRadius_ = minInnerRadius_;
                ringRenderer->GetPrimitive().outerRadius_ = minOuterRadius_;
                ringRenderer->GetTransform().UpdateMatrix();
            }

            shockWaveState_.currentTime = 0.f; // 時間をリセット
        }

        shockWaveState_.currentTime += GetMainDeltaTime();
        float t = shockWaveState_.currentTime / shockWaveState_.maxTime;

        if (t >= 1.f) {
            t = 1.f;
            shockWaveState_.playState.Set(false);
        }

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

            float innerRadius                         = std::lerp(minInnerRadius_, maxInnerRadius_, easeT);
            float outerRadius                         = std::lerp(minOuterRadius_, maxOuterRadius_, easeT);
            ringRenderer->GetPrimitive().innerRadius_ = innerRadius;
            ringRenderer->GetPrimitive().outerRadius_ = outerRadius;
            TextureMesh* mesh                         = &ringRenderer->GetMeshGroup()->back();
            ringRenderer->CreateMesh(mesh);
        }
    } else if (!shockWaveState_.playState.IsRelease()) {
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
            ringRenderer->GetTransform().translate[Y] = -10000.f;
            ringRenderer->GetTransform().UpdateMatrix();
        }
    }
}
