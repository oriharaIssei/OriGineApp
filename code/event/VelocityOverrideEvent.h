#pragma once

/// ECS
// component
#include "component/ComponentHandle.h"

/// math
#include "math/Vector3.h"

/// <summary>
/// Velocity を上書きするイベント
/// </summary>
struct VelocityOverrideEvent {
    VelocityOverrideEvent() = default;
    VelocityOverrideEvent(OriGine::ComponentHandle _rigidbodyHandle, const OriGine::Vector3f& _newVelocity)
        : rigidbodyHandle(_rigidbodyHandle), newVelocity(_newVelocity) {}

    OriGine::ComponentHandle rigidbodyHandle; // 変更対象の Rigidbody コンポーネントハンドル
    OriGine::Vector3f newVelocity; // 新しい速度ベクトル
};
