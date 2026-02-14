#include "TimeScaleEffectSystem.h"

/// engine
#include "Engine.h"
#include "scene/Scene.h"

/// ECS
// component
#include "component/effect/post/GrayscaleComponent.h"
#include "component/gimmick/TimeScaleEffectComponent.h"

using namespace OriGine;

TimeScaleEffectSystem::TimeScaleEffectSystem() : ISystem(SystemCategory::Effect) {}
TimeScaleEffectSystem::~TimeScaleEffectSystem() {}

void TimeScaleEffectSystem::Initialize() {
    DeltaTimer* deltaTimer = Engine::GetInstance()->GetDeltaTimer();

    deltaTimer->SetTimeScale("Player", 1.f);
    deltaTimer->SetTimeScale("Effect", 1.f);
    deltaTimer->SetTimeScale("Camera", 1.f);
    deltaTimer->SetTimeScale("Timer", 1.f);
}
void TimeScaleEffectSystem::Finalize() {}

void TimeScaleEffectSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    auto* timeScaleEffect = GetComponent<TimeScaleEffectComponent>(_handle);
    if (!timeScaleEffect || !timeScaleEffect->IsActive()) {
        return;
    }

    DeltaTimer* deltaTimer = Engine::GetInstance()->GetDeltaTimer();
    float deltaTime        = deltaTimer->GetDeltaTime();

    float elapsed = timeScaleEffect->GetTimeScaleElapsed() + deltaTime;

    // タイムスケールの更新
    for (auto& key : timeScaleEffect->GetTimeScaleTags()) {
        deltaTimer->SetTimeScale(key, timeScaleEffect->GetTimeScale());
    }

    // effectの更新
    GrayscaleComponent* grayscaleEffect = GetComponent<GrayscaleComponent>(_handle);
    if (grayscaleEffect) {
        float fadeInDuration  = timeScaleEffect->GetFadeInDuration();
        float fadeOutDuration = timeScaleEffect->GetFadeOutDuration();
        float totalDuration   = timeScaleEffect->GetTimeScaleDuration();

        float fadeOutStart = totalDuration - fadeOutDuration;

        if (elapsed < fadeInDuration) {
            float t = elapsed / fadeInDuration;
            grayscaleEffect->SetAmount(std::clamp(t, 0.0f, 1.0f));
        } else if (elapsed >= fadeOutStart) {
            float t = (elapsed - fadeOutStart) / fadeOutDuration;
            grayscaleEffect->SetAmount(std::clamp(1.0f - t, 0.0f, 1.0f));
        } else {
            grayscaleEffect->SetAmount(1.0f);
        }
    }

    timeScaleEffect->SetTimeScaleElapsed(elapsed);
    float t = elapsed / timeScaleEffect->GetTimeScaleDuration();
    if (t >= 1.0f) {
        // タイムスケールのリセット
        for (auto& key : timeScaleEffect->GetTimeScaleTags()) {
            deltaTimer->SetTimeScale(key, 1.0f);
        }
        // エンティティの削除
        GetScene()->AddDeleteEntity(_handle);
    }
}
