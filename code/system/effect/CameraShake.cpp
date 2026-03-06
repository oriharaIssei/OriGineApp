#include "CameraShake.h"

/// engine
#include "Engine.h"

#include "camera/CameraManager.h"

/// ECS
// component
#include "component/Camera/CameraShakeSourceComponent.h"
#include "component/transform/CameraTransform.h"

/// math
#include "math/Noise.h"
#include "math/SpringDamper.h"

#include "EffectConfig.h"

using namespace OriGine;

CameraShake::CameraShake() : ISystem(OriGine::SystemCategory::Effect) {}
CameraShake::~CameraShake() {}

void CameraShake::Initialize() {}
void CameraShake::Finalize() {}

void CameraShake::UpdateEntity(OriGine::EntityHandle _handle) {
    auto& cameraShakeSources = GetComponents<CameraShakeSourceComponent>(_handle);

    const float deltaTime = Engine::GetInstance()->GetDeltaTimer()->GetScaledDeltaTime("Effect");

    // CameraShakeSourceComponentが存在しない場合は Skip
    if (cameraShakeSources.empty()) {
        return;
    }
    for (auto& cameraShakeSource : cameraShakeSources) {
        // シェイクの適用先カメラTransformを取得
        auto cameraTransform = GetComponent<CameraTransform>(_handle, cameraShakeSource.cameraTransformIndex);
        if (!cameraTransform) {
            continue;
        }

        if (!cameraShakeSource.isActive) {
            continue;
        }

        // シェイクの継続時間処理
        cameraShakeSource.elapsedTime += deltaTime;
        if (!cameraShakeSource.isLoop) {
            if (cameraShakeSource.duration <= cameraShakeSource.elapsedTime) {
                // シェイク終了
                cameraShakeSource.isActive = false;
                continue;
            }
        }

        // シェイクオフセット計算
        OriGine::Vec3f shakeOffset(0.0f, 0.0f, 0.0f);
        switch (cameraShakeSource.type) {
        case ShakeSourceType::SinCurve:
            for (size_t i = 0; i < 3; ++i) {
                shakeOffset[i] = sinf(cameraShakeSource.elapsedTime * cameraShakeSource.axisParameters[i].frequency) * cameraShakeSource.axisParameters[i].amplitude;
            }
            break;
        case ShakeSourceType::Noise:
            for (size_t i = 0; i < 3; ++i) {
                float time = cameraShakeSource.elapsedTime * cameraShakeSource.axisParameters[i].frequency;

                OriGine::Vec2f st(
                    time, // 時間で動かす
                    time * EffectConfig::CameraShake::kNoiseCoefficient // 少しずらす。軸ごとに違う揺れになる
                );

                // ノイズを -1 ～ +1 に補正
                float f = FbmNoise::Fbm(st) - EffectConfig::CameraShake::kNoiseOffset;

                shakeOffset[i] = f * cameraShakeSource.axisParameters[i].amplitude;
            }
            break;
        case ShakeSourceType::Spring:
            for (size_t i = 0; i < 3; ++i) {
                // frequency を smoothTime に変換 (高い frequency = 速い収束)
                float smoothTime                    = (cameraShakeSource.axisParameters[i].frequency > 0.0f)
                                                          ? 1.0f / cameraShakeSource.axisParameters[i].frequency
                                                          : 1.0f;
                cameraShakeSource.springPosition[i] = OriGine::SmoothDamp(
                    cameraShakeSource.springPosition[i],
                    0.0f,
                    cameraShakeSource.springVelocity[i],
                    smoothTime,
                    deltaTime,
                    OriGine::kInfinity);
                shakeOffset[i] = cameraShakeSource.springPosition[i];
            }
            // バネのエネルギーが十分小さい場合は自動停止
            if (cameraShakeSource.springPosition.length() < 1e-4f && cameraShakeSource.springVelocity.length() < 1e-4f) {
                cameraShakeSource.isActive = false;
            }
            break;
        default:
            break;
        }

        // シェイクオフセット適用
        cameraTransform->translate += shakeOffset;
        cameraTransform->UpdateMatrix();
        CameraManager::GetInstance()->SetTransform(GetScene(), *cameraTransform);
    }
}
