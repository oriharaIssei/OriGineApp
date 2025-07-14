#include "EffectOnPlayerGearup.h"

/// Engine
#define ENGINE_ECS
#define DELTA_TIME
#include "EngineInclude.h"

// component
#include "component/effect/particle/emitter/Emitter.h"
#include "component/effect/post/RadialBlurParam.h"
#include "component/Player/PlayerStatus.h"

/// math
#include <math/MyEasing.h>

EffectOnPlayerGearup::EffectOnPlayerGearup() : ISystem(SystemCategory::Effect) {}

EffectOnPlayerGearup::~EffectOnPlayerGearup() {}

void EffectOnPlayerGearup::Initialize() {}

void EffectOnPlayerGearup::Finalize() {}

void EffectOnPlayerGearup::UpdateEntity(GameEntity* _entity) {
    GameEntity* player         = getUniqueEntity("Player");
    PlayerStatus* playerStatus = getComponent<PlayerStatus>(player);

    // ギアアップの衝撃波を生成
    DistortionEffectParam* distortionEffectParam = getComponent<DistortionEffectParam>(_entity);
    auto& shockWaveRings                         = distortionEffectParam->getDistortionObjects();
    RadialBlurParam* radialBlurParam             = getComponent<RadialBlurParam>(_entity);

    // 前フレームにPlayerがGearupしていたかどうか
    if (playerStatus->isGearUp()) {

        Transform* playerTransform = getComponent<Transform>(player);
        Transform* transform       = getComponent<Transform>(_entity);
        transform->translate       = playerTransform->translate; // Playerの位置に合わせる
        Matrix4x4 rotateMat        = MakeMatrix::RotateQuaternion(playerTransform->rotate);

        // Emitterの再生
        int32_t emitterSize = ECSManager::getInstance()->getComponentArray<Emitter>()->getComponentSize(_entity);
        for (int32_t i = 0; i < emitterSize; ++i) {
            Emitter* emitter = getComponent<Emitter>(_entity, i);
            if (emitter) {
                emitter->setOriginePos(emitterOffset_);
                emitter->PlayStart();
            }
        }

        if (playerStatus->getGearLevel() >= 3 && playerStatus->getGearLevel() % 2 == 0) {
            isEffectPlaying_ = true; // ギアアップの衝撃波を再生する

            // shockWaveRingの初期化Add commentMore actions
            for (auto& [object, type] : shockWaveRings) {
                if (type != PrimitiveType::Ring) {
                    continue;
                }
                if (object == nullptr) {
                    continue;
                }

                RingRenderer* ringRenderer          = dynamic_cast<RingRenderer*>(object.get());
                ringRenderer->getTransform().parent = playerTransform; // PlayerのTransformを親に設定

                ringRenderer->getTransform().translate = shockWaveOffset_; // 初期位置を設定
                ringRenderer->getTransform().rotate    = playerTransform->rotate;
                ringRenderer->getPrimitive().setInnerRadius(minInnerRadius_);
                ringRenderer->getPrimitive().setOuterRadius(minouterRadius_);
            }

            // radialBlurParam->Play(); // レイディアルブラーを有効にする
            currentTime_ = 0.f; // 時間をリセットAdd commentMore actions
        }
    }

    if (!isEffectPlaying_) {
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

        if (radialBlurParam) {
            radialBlurParam->Stop(); // レイディアルブラーを無効にする
        }
        return;
    }

    currentTime_ += getMainDeltaTime();
    float t = currentTime_ / maxTime_;

    if (t >= 1.f) {
        isEffectPlaying_ = false;
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

        // レイディアルブラーの幅を設定
        if (radialBlurParam) {
            float halfMaxTime = maxTime_ * 0.5f;
            if (currentTime_ < halfMaxTime) {
                radialBlurParam->setWidth(std::lerp(minRadialBlurWidth_, maxRadialBlurWidth_, EaseOutCubic(currentTime_ / halfMaxTime)));
            } else {
                radialBlurParam->setWidth(std::lerp(maxRadialBlurWidth_, minRadialBlurWidth_, EaseInOutQuad((currentTime_ - halfMaxTime) / halfMaxTime)));
            }
        }
    }
}
