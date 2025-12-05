#include "EffectOnPlayerRun.h"

/// Engine
#define DELTA_TIME
#include "EngineInclude.h"

/// ECS
// component
#include "component/renderer/MeshRenderer.h"
#include "component/renderer/primitive/CylinderRenderer.h"

#include "component/effect/particle/emitter/Emitter.h"
#include "component/effect/post/SpeedlineEffectParam.h"
#include "component/player/PlayerEffectControlParam.h"

#include "component/physics/Rigidbody.h"
#include "component/transform/Transform.h"

#include "component/player/PlayerInput.h"
#include "component/player/PlayerStatus.h"
#include "component/player/state/PlayerState.h"

/// math
#include "math/mathEnv.h"
#include "math/MyEasing.h"

EffectOnPlayerRun::EffectOnPlayerRun() : ISystem(SystemCategory::Effect) {}
EffectOnPlayerRun::~EffectOnPlayerRun() {}

void EffectOnPlayerRun::Initialize() {}
void EffectOnPlayerRun::Finalize() {}

void EffectOnPlayerRun::UpdateEntity(Entity* entity) {
    auto state              = GetComponent<PlayerState>(entity);
    auto status             = GetComponent<PlayerStatus>(entity);
    auto effectControlParam = GetComponent<PlayerEffectControlParam>(entity);
    auto rigidbody          = GetComponent<Rigidbody>(entity);

    if (!state || !effectControlParam || !rigidbody) {
        return;
    }

    // タイヤの回転
    ModelMeshRenderer* modelMeshRenderer = GetComponent<ModelMeshRenderer>(entity);
    if (modelMeshRenderer) {
        Transform& meshTransform = modelMeshRenderer->GetTransform(0);

        auto playerInput = GetComponent<PlayerInput>(entity);
        Vec3f inputDir   = playerInput->GetWorldInputDirection();

        // 速度によって タイヤの回転速度を変える
        // 入力方向ベクトルが0のときは 回転しないようにする(Sqなのは,0か1しかないので軽くするため)
        const Vec3f& velo    = rigidbody->GetVelocity();
        float wheelSpinSpeed = effectControlParam->CalculateWheelSpinSpeedBySpeed(velo.length() * inputDir.lengthSq(), status->CalculateSpeedByGearLevel(kMaxPlayerGearLevel));
        meshTransform.rotate *= Quaternion::RotateAxisAngle(axisX, wheelSpinSpeed);
        meshTransform.UpdateMatrix();

        // プレイヤーが曲がる時,タイヤを傾ける (地面にいるときだけ)
        Transform* hostTransform = GetComponent<Transform>(entity);
        if (state->IsOnGround()) {
            if (inputDir.lengthSq() > kEpsilon) {
                Vec3f currentDir = hostTransform->rotate.RotateVector(axisZ);
                currentDir[Y]    = 0.f;
                currentDir       = currentDir.normalize();

                // 現在の傾き
                float wheelTiltAngle = effectControlParam->CalculateWheelTiltAngle(inputDir, currentDir);

                // 段々と傾く(1秒に傾く角度が制限されている)
                float preWheelTiltAngle = effectControlParam->GetPreWheelTiltAngle();
                float angleDiff         = wheelTiltAngle - preWheelTiltAngle;
                float maxAngleChange    = effectControlParam->GetWheelTiltAngleMaxAccel() * GetMainDeltaTime();
                wheelTiltAngle          = preWheelTiltAngle + std::clamp(angleDiff, -maxAngleChange, maxAngleChange);

                // 傾きを適用
                hostTransform->rotate *= Quaternion::RotateAxisAngle(axisZ, wheelTiltAngle);
                hostTransform->UpdateMatrix();

                effectControlParam->SetPreWheelTiltAngle(wheelTiltAngle);
            }
        }
    }

    // trailの色をGearLevelに応じて変化
    Vector4f trailColor = effectControlParam->GetTrailColorByGearLevel(state->GetGearLevel());
    Entity* trailEntity = GetUniqueEntity("Trail");
    if (effectControlParam) {
        if (trailEntity) {
            Material* material = GetComponent<Material>(trailEntity);
            if (material) {
                material->color_ = trailColor;
                material->UpdateUvMatrix();
            }
        }

        // BackFire を 速度によって 強さを変える
        constexpr float kMinBackFireScaleY  = 0.36f;
        constexpr float kMaxBackFireScaleY  = 1.89f;
        constexpr float kMinBackFireScaleXZ = 0.74f;
        constexpr float kMaxBackFireScaleXZ = 1.56f;

        // BackFire の色をGearLevelに応じて変化
        Entity* backFireEntity       = GetUniqueEntity("BackFire");
        Entity* backFireSparksEntity = GetUniqueEntity("BackFireSparks");

        if (backFireEntity && backFireSparksEntity) {
            Material* material = GetComponent<Material>(backFireEntity);
            material->color_   = trailColor;

            material         = GetComponent<Material>(backFireSparksEntity);
            material->color_ = trailColor;

            auto* backFireCylinders       = GetComponents<CylinderRenderer>(backFireEntity);
            auto* backFireSparksCylinders = GetComponents<CylinderRenderer>(backFireSparksEntity);

            float currentSpeed = rigidbody->GetVelocity().length();
            float t            = currentSpeed / status->CalculateSpeedByGearLevel(kMaxPlayerGearLevel);
            t                  = EaseInCubic(t);
            float xzScale      = std::lerp(kMinBackFireScaleXZ, kMaxBackFireScaleXZ, t);
            float yScale       = std::lerp(kMinBackFireScaleY, kMaxBackFireScaleY, t);

            Vec3f newScale = Vec3f(xzScale, yScale, xzScale);
            for (auto& cylinder : *backFireCylinders) {
                cylinder.GetTransform().scale = newScale;
                cylinder.GetTransform().UpdateMatrix();
            }
            for (auto& cylinder : *backFireSparksCylinders) {
                cylinder.GetTransform().scale = newScale;
                cylinder.GetTransform().UpdateMatrix();
            }
        }
    }
    // スピードラインエフェクト
    Entity* speedlineEntity                            = GetUniqueEntity("Speedline");
    std::vector<SpeedlineEffectParam>* speedlineParams = nullptr;
    if (speedlineEntity) {
        speedlineParams = GetComponents<SpeedlineEffectParam>(speedlineEntity);
    }

    if (!speedlineParams) {
        return;
    }

    // プレイヤーが止まっているか, ギアレベルが低い場合は エフェクトを停止
    if (state->GetStateEnum() == PlayerMoveState::IDLE || state->GetGearLevel() < 2) {
        for (auto& speedlineParam : *speedlineParams) {
            speedlineParam.Stop();
        }

        return;
    }

    // 速度によって エフェクトの強さを変える
    float intensityT = static_cast<float>(state->GetGearLevel()) / static_cast<float>(kMaxPlayerGearLevel);
    intensityT       = EaseOutCubic(intensityT);
    float intensity  = std::lerp(0.f, maxIntensity_, intensityT);
    for (auto& speedlineParam : *speedlineParams) {
        speedlineParam.Play();
        auto& paramData = speedlineParam.GetParamData();
        paramData.time -= GetMainDeltaTime();
        paramData.intensity = std::lerp(paramData.intensity, intensity, 0.1f);
    }
}
