#include "EffectOnPlayerRun.h"

/// Engine
#define DELTA_TIME
#include "EngineInclude.h"

/// ECS
// component
#include "component/effect/particle/emitter/Emitter.h"
#include "component/effect/post/SpeedlineEffectParam.h"
#include "component/Player/State/PlayerState.h"

/// math
#include "math/MyEasing.h"

EffectOnPlayerRun::EffectOnPlayerRun() : ISystem(SystemCategory::Effect) {}
EffectOnPlayerRun::~EffectOnPlayerRun() {}

void EffectOnPlayerRun::Initialize() {}
void EffectOnPlayerRun::Finalize() {}

void EffectOnPlayerRun::UpdateEntity(Entity* entity) {
    auto state = getComponent<PlayerState>(entity);
    if (state == nullptr) {
        return;
    }

    auto playerEmitterEntity             = getUniqueEntity("PlayerEmitter");
    std::vector<Emitter>* playerEmitters = nullptr;
    if (playerEmitterEntity) {
        playerEmitters = getComponents<Emitter>(playerEmitterEntity);
    }

    auto speedlineParams = getComponents<SpeedlineEffectParam>(getUniqueEntity("Speedline"));

    if (!speedlineParams || !playerEmitters) {
        return;
    }
    if (state->getStateEnum() == PlayerMoveState::IDLE || state->getGearLevel() < 2) {
        for (auto& speedlineParam : *speedlineParams) {
            speedlineParam.Stop();
        }
        for (auto& emitter : *playerEmitters) {
            emitter.setLeftActiveTime(0.f);
            emitter.setIsLoop(false);
        }

        return;
    }

    // 速度によって エフェクトの強さを変える
    float intensityT = static_cast<float>(state->getGearLevel()) / static_cast<float>(kMaxPlayerGearLevel);
    intensityT       = EaseOutCubic(intensityT);
    float intensity  = std::lerp(0.f, maxIntensity, intensityT);
    for (auto& speedlineParam : *speedlineParams) {
        speedlineParam.Play();
        auto& paramData = speedlineParam.getParamData();
        paramData.time -= getMainDeltaTime();
        paramData.intensity = std::lerp(paramData.intensity, intensity, 0.1f);
    }

    // エミッターも再生
    constexpr Vec3f emitterOffset = Vec3f(0.f, -0.46f, -0.687f);
    Transform* playerTransform    = getComponent<Transform>(entity);

    Vec3f offset = emitterOffset * MakeMatrix::RotateQuaternion(playerTransform->rotate);
    for (auto& emitter : *playerEmitters) {
        if (!emitter.IsActive()) {
            emitter.PlayStart();
            emitter.setLeftActiveTime(1.f);
            emitter.setIsLoop(true);
        }

        emitter.PlayContinue();
        emitter.setOriginPos(playerTransform->translate + offset);
    }
}
