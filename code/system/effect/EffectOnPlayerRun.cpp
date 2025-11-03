#include "EffectOnPlayerRun.h"

/// Engine
#define DELTA_TIME
#include "EngineInclude.h"

/// ECS
// component
#include "component/renderer/MeshRenderer.h"

#include "component/effect/particle/emitter/Emitter.h"
#include "component/effect/post/SpeedlineEffectParam.h"
#include "component/Player/PlayerEffectControlParam.h"

#include "component/physics/Rigidbody.h"
#include "component/transform/Transform.h"

#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"

/// math
#include "math/mathEnv.h"
#include "math/MyEasing.h"

EffectOnPlayerRun::EffectOnPlayerRun() : ISystem(SystemCategory::Effect) {}
EffectOnPlayerRun::~EffectOnPlayerRun() {}

void EffectOnPlayerRun::Initialize() {}
void EffectOnPlayerRun::Finalize() {}

void EffectOnPlayerRun::UpdateEntity(Entity* entity) {
    auto state              = getComponent<PlayerState>(entity);
    auto status             = getComponent<PlayerStatus>(entity);
    auto effectControlParam = getComponent<PlayerEffectControlParam>(entity);
    auto rigidbody          = getComponent<Rigidbody>(entity);

    if (!state || !effectControlParam || !rigidbody) {
        return;
    }

    // タイヤの回転
    ModelMeshRenderer* modelMeshRenderer = getComponent<ModelMeshRenderer>(entity);
    if (modelMeshRenderer) {
        Transform& meshTransform = modelMeshRenderer->getTransform(0);

        auto playerInput = getComponent<PlayerInput>(entity);
        Vec3f inputDir   = playerInput->getWorldInputDirection();

        // 速度によって タイヤの回転速度を変える
        // 入力方向ベクトルが0のときは 回転しないようにする(Sqなのは,0か1しかないので軽くするため)
        const Vec3f& velo    = rigidbody->getVelocity();
        float wheelSpinSpeed = effectControlParam->CalculateWheelSpinSpeedBySpeed(velo.length() * inputDir.lengthSq(), status->CalculateSpeedByGearLevel(kMaxPlayerGearLevel));
        meshTransform.rotate *= Quaternion::RotateAxisAngle(axisX, wheelSpinSpeed);
        meshTransform.UpdateMatrix();

        // プレイヤーが曲がる時,タイヤを傾ける (地面にいるときだけ)
        Transform* hostTransform = getComponent<Transform>(entity);
        if (state->isOnGround()) {
            if (inputDir.lengthSq() > kEpsilon) {
                Vec3f currentDir = hostTransform->rotate.RotateVector(axisZ);
                currentDir[Y]    = 0.f;
                currentDir       = currentDir.normalize();

                // 現在の傾き
                float wheelTiltAngle = effectControlParam->CalculateWheelTiltAngle(inputDir, currentDir);

                // 段々と傾く(1秒に傾く角度が制限されている)
                float preWheelTiltAngle = effectControlParam->getPreWheelTiltAngle();
                float angleDiff         = wheelTiltAngle - preWheelTiltAngle;
                float maxAngleChange    = effectControlParam->getWheelTiltAngleMaxAccel() * getMainDeltaTime();
                wheelTiltAngle          = preWheelTiltAngle + std::clamp(angleDiff, -maxAngleChange, maxAngleChange);

                // 傾きを適用
                hostTransform->rotate *= Quaternion::RotateAxisAngle(axisZ, wheelTiltAngle);
                hostTransform->UpdateMatrix();

                effectControlParam->setPreWheelTiltAngle(wheelTiltAngle);
            }
        }
    }

    Entity* speedlineEntity                            = getUniqueEntity("Speedline");
    std::vector<SpeedlineEffectParam>* speedlineParams = nullptr;
    if (speedlineEntity) {
        speedlineParams = getComponents<SpeedlineEffectParam>(speedlineEntity);
    }

    if (!speedlineParams) {
        return;
    }

    // プレイヤーが止まっているか, ギアレベルが低い場合は エフェクトを停止
    if (state->getStateEnum() == PlayerMoveState::IDLE || state->getGearLevel() < 2) {
        for (auto& speedlineParam : *speedlineParams) {
            speedlineParam.Stop();
        }

        return;
    }

    // 速度によって エフェクトの強さを変える
    float intensityT = static_cast<float>(state->getGearLevel()) / static_cast<float>(kMaxPlayerGearLevel);
    intensityT       = EaseOutCubic(intensityT);
    float intensity  = std::lerp(0.f, maxIntensity_, intensityT);
    for (auto& speedlineParam : *speedlineParams) {
        speedlineParam.Play();
        auto& paramData = speedlineParam.getParamData();
        paramData.time -= getMainDeltaTime();
        paramData.intensity = std::lerp(paramData.intensity, intensity, 0.1f);
    }
}
