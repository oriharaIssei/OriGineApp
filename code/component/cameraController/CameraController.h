#pragma once

/// parent
#include "component/IComponent.h"

/// engine
// component
#include "component/transform/Transform.h"

/// math
#include "math/Vector2.h"
#include "math/Vector3.h"

class CameraController
    : public IComponent {
    friend void to_json(nlohmann::json& j, const CameraController& _cameraController);
    friend void from_json(const nlohmann::json& j, CameraController& _cameraController);

public:
    CameraController() {}
    ~CameraController() override {}

    void Initialize(GameEntity* _entity) override;

    bool Edit() override;

    void Finalize() override;

    float CalculateFovY(int32_t _level);

private:
    Transform* followTarget_ = nullptr;

    Vec3f forward_     = {0.f, 0.f, 1.f}; // カメラの前方向
    float angleLimitY_ = 0.0f;

    /// <summary>
    /// 注視点のOffset
    /// </summary>
    Vec3f followTargetOffset_ = {0.f, 0.f, 0.f};
    /// <summary>
    /// 注視点との距離のOffset
    /// </summary>
    Vec3f followOffset_             = {0.f, 0.f, 0.f};
    Vec3f interTarget_              = {0.f, 0.f, 0.f};
    Vec2f destinationAngleXY_       = {0.0f, 0.0f};
    float rotateSpeedPadStick_      = 0.0f;
    float rotateSpeedMouse_         = 0.0f;
    float rotateSensitivity_        = 0.0f;
    float interTargetInterpolation_ = 0.0f;

    float maxRotateX_ = 0.0f;
    float minRotateX_ = 0.0f;

    float fovYInterpolate_    = 0.1f;
    float baseFovY_           = 45.0f * 3.141592654f / 180.0f; // 基準のFovY
    float fovYRateBase_       = 1.0f; // FovYの倍率
    float fovYRateCommonRate_ = 1.0f; // FovYの倍率の共通値

public:
    const Vec3f& getForward() const { return forward_; }
    void setForward(const Vec3f& forward) { forward_ = forward; }

    float getAngleLimitY() const { return angleLimitY_; }
    void setAngleLimitY(float angleLimitY) { angleLimitY_ = angleLimitY; }

    const Vec3f& getFollowTargetOffset() const { return followTargetOffset_; }
    void setFollowTargetOffset(const Vec3f& offset) { followTargetOffset_ = offset; }

    const Vec3f& getFollowOffset() const { return followOffset_; }
    void setFollowOffset(const Vec3f& offset) { followOffset_ = offset; }

    const Vec3f& getInterTarget() const { return interTarget_; }
    void setInterTarget(const Vec3f& target) { interTarget_ = target; }

    const Vec2f& getDestinationAngleXY() const { return destinationAngleXY_; }
    void setDestinationAngleXY(const Vec2f& angle) { destinationAngleXY_ = angle; }

    float getRotateSpeedMouse() const { return rotateSpeedMouse_; }
    void setRotateSpeedMouse(float speed) { rotateSpeedMouse_ = speed; }

    float getRotateSpeedPadStick() const { return rotateSpeedPadStick_; }
    void setRotateSpeedPadStick(float speed) { rotateSpeedPadStick_ = speed; }

    float getRotateSensitivity() const { return rotateSensitivity_; }
    void setRotateSensitivity(float sensitivity) { rotateSensitivity_ = sensitivity; }

    float getInterTargetInterpolation() const { return interTargetInterpolation_; }
    void setInterTargetInterpolation(float interpolation) { interTargetInterpolation_ = interpolation; }

    float getMaxRotateX() const { return maxRotateX_; }
    void setMaxRotateX(float maxRotateX) { maxRotateX_ = maxRotateX; }

    float getMinRotateX() const { return minRotateX_; }
    void setMinRotateX(float minRotateX) { minRotateX_ = minRotateX; }

    const Transform* getFollowTarget() const { return followTarget_; }
    void setFollowTarget(Transform* target) { followTarget_ = target; }

    float getFovYInterpolate() const { return fovYInterpolate_; }
};
