#pragma once

/// ECS
// component
#include "component/physics/Rigidbody.h"
#include "PlayerStatus.h"

namespace PlayerMoveUtils {

/// <summary>
/// Playerが壁に接触した際の挙動を評価する
/// </summary>
enum class WallContactResult {
    WallRun,
    Wheelie,
    WallHit
};

/// <summary>
/// Player が壁に接触した際の挙動を評価する
/// </summary>
/// <param name=""></param>
/// <returns></returns>
WallContactResult EvaluateWallContact(
    float _parallelFactor,
    const OriGine::Rigidbody* _rigidbody,
    const PlayerStatus* _status);

} // namespace PlayerMoveUtils
