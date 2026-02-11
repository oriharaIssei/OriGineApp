#include "ApplySpeedModifiers.h"

/// engine
#include "Engine.h"
#include "scene/Scene.h"

/// ECS
// component
#include "component/physics/Rigidbody.h"
#include "component/SpeedModifiers.h"

/// math
#include "math/Interpolation.h"
#include <MyEasing.h>
#include <Vector3.h>

using namespace OriGine;

ApplySpeedModifiers::ApplySpeedModifiers() : ISystem(SystemCategory::Movement) {}
ApplySpeedModifiers::~ApplySpeedModifiers() {}

void ApplySpeedModifiers::Initialize() {}
void ApplySpeedModifiers::Finalize() {}

void ApplySpeedModifiers::UpdateEntity(OriGine::EntityHandle _handle) {
    auto& speedModifiers = GetComponents<SpeedModifiers>(_handle);

    float deltaTime    = 0.f;
    float defDeltaTime = Engine::GetInstance()->GetDeltaTimer()->GetDeltaTime();

    for (auto& speedModifier : speedModifiers) {
        // 対象のRigidbodyコンポーネントを取得
        auto rigidbodyComp = GetComponent<Rigidbody>(speedModifier.targetRigidbodyHandle);
        if (!rigidbodyComp) {
            continue;
        }

        if (rigidbodyComp->IsUsingLocalDeltaTime()) {
            deltaTime = Engine::GetInstance()->GetDeltaTimer()->GetScaledDeltaTime(rigidbodyComp->GetLocalDeltaTimeName());
        } else {
            deltaTime = defDeltaTime;
        }

        float t               = 0.f;
        Vec3f additiveSpeed   = kZeroVec3f;
        Vec3f multiplierSpeed = kOneVec3f;

        speedModifier.additiveTimer += deltaTime;
        speedModifier.additiveLerpTimer += deltaTime;
        if (speedModifier.additiveLerpTimer <= speedModifier.additiveLerpDuration) {
            t             = std::min(speedModifier.additiveLerpTimer / speedModifier.additiveLerpDuration, 1.0f);
            additiveSpeed = Lerp<3, float>(kZeroVec3f, speedModifier.additiveTarget, EasingFunctions[static_cast<int>(speedModifier.additiveLerpEaseType)](t));
        } else {
            additiveSpeed = LerpByDeltaTime<Vec3f>(speedModifier.additiveTarget, kZeroVec3f, deltaTime, speedModifier.restoreSpeed);
        }

        speedModifier.multiplierTimer += deltaTime;
        speedModifier.multiplierLerpTimer += deltaTime;
        if (speedModifier.multiplierLerpTimer <= speedModifier.multiplierLerpDuration) {
            t               = std::min(speedModifier.multiplierTimer / speedModifier.multiplierLerpDuration, 1.0f);
            multiplierSpeed = Lerp<3, float>(kOneVec3f, speedModifier.multiplierTarget, EasingFunctions[static_cast<int>(speedModifier.multiplierLerpEaseType)](t));
        } else {
            multiplierSpeed = LerpByDeltaTime<Vec3f>(speedModifier.multiplierTarget, kOneVec3f, deltaTime, speedModifier.restoreSpeed);
        }

        Vec3f effectiveSpeed = (speedModifier.beforeSpeed * multiplierSpeed) + additiveSpeed;
        rigidbodyComp->SetMaxXZSpeed(effectiveSpeed.length());
        rigidbodyComp->SetVelocity(rigidbodyComp->GetVelocity().normalize() * effectiveSpeed);

        // 自動削除
        if (speedModifier.isAutoDestroyed) {
            if (!speedModifier.IsAdditiveEffectActive() && !speedModifier.IsMultiplierEffectActive()) {
                GetScene()->AddDeleteEntity(_handle);
            }
        }
    }
}
