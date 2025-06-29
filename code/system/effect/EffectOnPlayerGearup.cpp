#include "EffectOnPlayerGearup.h"

/// Engine
#define ENGINE_ECS
#define DELTA_TIME
#include "EngineInclude.h"

// component
#include "component/effect/particle/emitter/Emitter.h"
#include "component/Player/PlayerStatus.h"

/// math
#include <math/MyEasing.h>

EffectOnPlayerGearup::EffectOnPlayerGearup() : ISystem(SystemType::Effect) {}

EffectOnPlayerGearup::~EffectOnPlayerGearup() {}

void EffectOnPlayerGearup::Initialize() {}

void EffectOnPlayerGearup::Finalize() {}

void EffectOnPlayerGearup::UpdateEntity(GameEntity* _entity) {
    GameEntity* player         = getUniqueEntity("Player");
    PlayerStatus* playerStatus = getComponent<PlayerStatus>(player);

    // ギアアップの衝撃波を生成
    DistortionEffectParam* distortionEffectParam = getComponent<DistortionEffectParam>(_entity);
    auto& shockWaveRings                         = distortionEffectParam->getDistortionObjects();

    // 前フレームにPlayerがGearupしていたかどうか
    if (playerStatus->isGearUp()) {
        // Emitterの再生
        Emitter* emitter = getComponent<Emitter>(_entity);
        if (emitter) {
            emitter->PlayStart();
        }

        if (playerStatus->getGearLevel() >= 3 && playerStatus->getGearLevel() % 2 == 0) {
            isRingAnimationPlay_ = true; // ギアアップの衝撃波を再生する

            // shockWaveRingの初期化Add commentMore actions
            for (auto& [object, type] : shockWaveRings) {
                if (type != PrimitiveType::Ring) {
                    continue;
                }
                if (object == nullptr) {
                    continue;
                }

                RingRenderer* ringRenderer = dynamic_cast<RingRenderer*>(object.get());

                // 初期位置
                Transform* playerTransform             = getComponent<Transform>(player);
                Vec3f spawnPos                         = Vec3f(playerTransform->translate + shockWaveOffset_) * MakeMatrix::RotateQuaternion(playerTransform->rotate);
                ringRenderer->getTransform().translate = spawnPos;
                ringRenderer->getTransform().rotate    = playerTransform->rotate;

                ringRenderer->getPrimitive().setInnerRadius(minInnerRadius_);
                ringRenderer->getPrimitive().setOuterRadius(minouterRadius_);
            }
        }
        currentTime_ = 0.f; // 時間をリセットAdd commentMore actions
    }

    if (!isRingAnimationPlay_) {
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
        return;
    }

    currentTime_ += getMainDeltaTime();
    float t = currentTime_ / maxTime_;

    if (t >= 1.f) {
        isRingAnimationPlay_ = false;
    }

    // Ring
    for (auto& [object, type] : shockWaveRings) {
        if (type != PrimitiveType::Ring) {
            continue;
        }
        if (object == nullptr) {
            continue;
        }

        RingRenderer* ringRenderer = dynamic_cast<RingRenderer*>(object.get());
        ringRenderer->getPrimitive().setInnerRadius(std::lerp(minInnerRadius_, maxInnerRadius_, EaseInOutQuad(t)));
        ringRenderer->getPrimitive().setOuterRadius(std::lerp(minouterRadius_, maxouterRadius_, EaseOutCubic(t)));

        ringRenderer->createMesh(&ringRenderer->getMeshGroup()->back());
    }
}
