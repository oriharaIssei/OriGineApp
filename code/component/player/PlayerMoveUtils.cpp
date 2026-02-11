#include "PlayerMoveUtils.h"

using namespace OriGine;

namespace PlayerMoveUtils {
WallContactResult EvaluateWallContact(float _parallelFactor, const Rigidbody* _rigidbody, const PlayerStatus* _status) {
    // 壁に沿って移動している場合は壁衝突とみなす
    if (_parallelFactor > _status->GetWallCheckThreshold()) {
        if (_status->CanWallRun()) {
            return WallContactResult::WallRun;
        }
    } else { // 基準値以下なら
        if (_rigidbody->GetVelocity()[Y] > _status->GetMaxWheelieFallSpeed()) {
            if (_status->CanWheelie()) {
                return WallContactResult::Wheelie;
            }
        }
    }
    return WallContactResult::WallHit;
}
}
