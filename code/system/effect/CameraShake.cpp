#include "CameraShake.h"

/// engine
#include "camera/CameraManager.h"

#define DELTA_TIME
#include "EngineInclude.h"

/// ECS
// component
#include "component/Camera/CameraShakeSourceComponent.h"
#include "component/transform/CameraTransform.h"

/// math
#include "math/Noise.h"

CameraShake::CameraShake() : ISystem(SystemCategory::Effect) {}
CameraShake::~CameraShake() {}

void CameraShake::Initialize() {}
void CameraShake::Finalize() {}

void CameraShake::UpdateEntity(Entity* _entity) {
    auto cameraShakeSources = GetComponents<CameraShakeSourceComponent>(_entity);

    // CameraShakeSourceComponentが存在しない場合は Skip
    if (!cameraShakeSources) {
        return;
    }
    for (auto& cameraShakeSource : *cameraShakeSources) {
        // シェイクの適用先カメラTransformを取得
        auto cameraTransform = GetComponent<CameraTransform>(_entity, cameraShakeSource.cameraTransformIndex);
        if (!cameraTransform) {
            continue;
        }

        if (!cameraShakeSource.isActive) {
            continue;
        }

        // シェイクの継続時間処理
        cameraShakeSource.elapsedTime += GetMainDeltaTime();
        if (!cameraShakeSource.isLoop) {
            if (cameraShakeSource.duration <= cameraShakeSource.elapsedTime) {
                // シェイク終了
                cameraShakeSource.isActive = false;
                continue;
            }
        }

        // シェイクオフセット計算
         Vec3f shakeOffset(0.0f, 0.0f, 0.0f);
        switch (cameraShakeSource.type) {
        case ShakeSourceType::SinCurve:
            for (size_t i = 0; i < 3; ++i) {
                shakeOffset[i] = sinf(cameraShakeSource.elapsedTime * cameraShakeSource.axisParameters[i].frequency) * cameraShakeSource.axisParameters[i].amplitude;
            }
            break;
        case ShakeSourceType::Noise:
            for (size_t i = 0; i < 3; ++i) {
                float time = cameraShakeSource.elapsedTime * cameraShakeSource.axisParameters[i].frequency;

                Vec2f st(
                    time, // 時間で動かす
                    time * 1.37f // 少しずらす。軸ごとに違う揺れになる
                );

                // ノイズを -1 ～ +1 に補正
                float f = FbmNoise::Fbm(st) - 0.5f;

                shakeOffset[i] = f * cameraShakeSource.axisParameters[i].amplitude;
            }
            break;
        default:
            break;
        }

        // シェイクオフセット適用
        cameraTransform->translate += shakeOffset;
        cameraTransform->UpdateMatrix();
        CameraManager::GetInstance()->SetTransform(*cameraTransform);
    }
}
