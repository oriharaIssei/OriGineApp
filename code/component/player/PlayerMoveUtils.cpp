#include "PlayerMoveUtils.h"

using namespace OriGine;

namespace PlayerMoveUtils {

bool IsHitGround(const Vec3f& _collNormal) {
    return _collNormal[Y] > kGroundCheckThreshold;
}

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

OriGine::Vec3f ComputeWallRunDirection(const OriGine::Vec3f& _velocity, const OriGine::Vec3f& _wallNormal) {
    return _velocity - _wallNormal * OriGine::Vec3f::Dot(_velocity, _wallNormal);
}

OriGine::Vec3f ComputeWheelieDirection(const OriGine::Vec3f& _wallNormal, const OriGine::Vec3f& _up) {
    float dot            = Vec3f::Dot(_wallNormal, _up);
    Vec3f climbDirection = axisY - (_wallNormal * dot);
    climbDirection       = climbDirection.normalize();
    return climbDirection;
}

bool IsWallRight(const OriGine::Vec3f& _direction, const OriGine::Vec3f& _wallNormal, const OriGine::Vec3f& _up) {
    return Vec3f::Dot(Vec3f::Cross(_up, _wallNormal), _direction) > 0.0f;
}

}
