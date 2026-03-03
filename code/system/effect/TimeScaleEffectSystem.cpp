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

    // タイムスケールの更新
    for (auto& key : timeScaleEffect->GetTimeScaleTags()) {
        stopTimesByTimeScale_[key].timeScale = timeScaleEffect->GetTimeScale();
        stopTimesByTimeScale_[key].scaleTimer += timeScaleEffect->GetTimeScaleDuration();
    }

    // エンティティの削除
    GetScene()->AddDeleteEntity(_handle);
}

void TimeScaleEffectSystem::Update() {
    if (!entities_.empty()) {
        EraseDeadEntity();

        for (auto& entityID : entities_) {
            UpdateEntity(entityID);
        }
    }

    // タイムスケールの適用
    DeltaTimer* deltaTimer = Engine::GetInstance()->GetDeltaTimer();
    for (const auto& [key, timeScaleComponent] : stopTimesByTimeScale_) {
        if (timeScaleComponent.scaleTimer > 0.f) {
            deltaTimer->SetTimeScale(key, timeScaleComponent.timeScale);
            // 経過時間を減らす
            stopTimesByTimeScale_[key].scaleTimer -= deltaTimer->GetDeltaTime();
        } else {
            // タイムスケールのリセット
            stopTimesByTimeScale_[key].scaleTimer = 0.f;
            deltaTimer->SetTimeScale(key, 1.0f);
        }
    }
}
