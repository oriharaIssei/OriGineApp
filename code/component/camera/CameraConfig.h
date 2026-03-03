#pragma once

#include "math/MathEnv.h"
#include "math/Vector2.h"
#include "math/Vector3.h"

namespace AppConfig {
namespace Camera {

constexpr float kDefaultTilt                     = 0.33f;
constexpr float kDefaultInterTargetInterpolation = 5.0f;
constexpr float kDefaultRotateSensitivity        = 10.0f;

// Offsets
constexpr OriGine::Vec3f kFirstOffset       = {0.0f, 0.0f, -10.0f};
constexpr OriGine::Vec3f kFirstTargetOffset = {0.0f, 0.0f, 1.0f};

constexpr OriGine::Vec3f kTargetOffsetOnDash = {0.0f, -0.5f, 1.0f};

constexpr OriGine::Vec3f kOffsetOnDash = {0.0f, 0.5f, -12.0f};

constexpr float kFixForForwardSpeed = 10.0f;

constexpr OriGine::Vec2f kDefaultDestinationAngle = {-0.1f, 0.0f};

// FOV
constexpr float kDefaultBaseFovY           = 45.0f;
constexpr float kDefaultFovYInterpolate    = 0.1f;
constexpr float kDefaultBaseFovYRad        = kDefaultBaseFovY * OriGine::kDeg2Rad;
constexpr float kDefaultFovYRateBase       = 1.0f;
constexpr float kDefaultFovYRateCommonRate = 1.0f;

} // namespace Camera
} // namespace AppConfig
