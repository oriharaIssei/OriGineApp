#include "EffectOnPlayerRun.h"

/// Engine
#define DELTA_TIME
#include "EngineInclude.h"

/// ECS
// component
#include "component/effect/post/SpeedlineEffectParam.h"
#include "component/Player/State/PlayerState.h"

/// math
#include "math/MyEasing.h"

EffectOnPlayerRun::EffectOnPlayerRun() : ISystem(SystemCategory::Effect) {}
EffectOnPlayerRun::~EffectOnPlayerRun() {}

void EffectOnPlayerRun::Initialize() {}
void EffectOnPlayerRun::Finalize() {}

void EffectOnPlayerRun::UpdateEntity(GameEntity* entity) {
    auto state = getComponent<PlayerState>(entity);
    if (state == nullptr) {
        return;
    }

    auto speedlineParams = getComponents<SpeedlineEffectParam>(getUniqueEntity("Speedline"));
    if (speedlineParams == nullptr) {
        return;
    }
    if (state->getStateEnum() == PlayerMoveState::IDLE || state->getGearLevel() < 2) {
        for (auto& speedlineParam : *speedlineParams) {
            speedlineParam.Stop();
        }
        return;
    }

    float intensityT = static_cast<float>(state->getGearLevel()) / static_cast<float>(kMaxPlayerGearLevel);
    intensityT       = EaseOutCubic(intensityT);
    float intensity  = std::lerp(0.f, maxIntensity, intensityT);
    for (auto& speedlineParam : *speedlineParams) {
        speedlineParam.Play();
        auto& paramData = speedlineParam.getParamData();
        paramData.time -= getMainDeltaTime();
        paramData.intensity = std::lerp(paramData.intensity, intensity, 0.1f);
    }
}
