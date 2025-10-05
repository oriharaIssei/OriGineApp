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

    void Edit(Scene* _scene, GameEntity* _entity, const std::string& _parentLabel) override;

    void Finalize() override;

    float CalculateFovY(int32_t _level) const;

private:
    Transform* followTarget_ = nullptr;

    Vec3f forward_     = {0.f, 0.f, 1.f}; // カメラの前方向
    float angleLimitY_ = 0.0f;

    /// <summary>
    /// 注視点のOffset
    /// </summary>
    Vec3f currentTargetOffset_   = {0.f, 0.f, 0.f};
    Vec3f firstTargetOffset_     = {0.f, 0.f, 0.f};
    Vec3f targetOffsetOnDash_    = {0.f, 0.f, 0.f};
    Vec3f targetOffsetOnWallRun_ = Vec3f(0.f, 0.f, 0.f);
    /// <summary>
    /// 注視点との距離のOffset
    /// </summary>
    Vec3f currentOffset_   = {0.f, 0.f, 0.f};
    Vec3f firstOffset_     = {0.f, 0.f, 0.f};
    Vec3f offsetOnDash_    = {0.f, 0.f, 0.f};
    Vec3f offsetOnWallRun_ = Vec3f(0.f, 0.f, 0.f);

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
    float fixForForwardSpeed_ = 0.1f; // 正面方向への 補正速度

public:
    const Vec3f& getForward() const { return forward_; }
    void setForward(const Vec3f& forward) { forward_ = forward; }

    float getAngleLimitY() const { return angleLimitY_; }
    void setAngleLimitY(float angleLimitY) { angleLimitY_ = angleLimitY; }

    const Vec3f& getCurrentTargetOffset() const { return currentTargetOffset_; }
    void setCurrentTargetOffset(const Vec3f& _offset) { currentTargetOffset_ = _offset; }

    const Vec3f& getFirstTargetOffset() const { return firstTargetOffset_; }
    void setFirstTargetOffset(const Vec3f& offset) { firstTargetOffset_ = offset; }

    const Vec3f& getTargetOffsetOnDash() const { return targetOffsetOnDash_; }
    void setTargetOffsetOnDash(const Vec3f& _offset) { targetOffsetOnDash_ = _offset; }

    const Vec3f& getTargetOffsetOnWallRun() const { return targetOffsetOnWallRun_; }
    void setTargetOffsetOnWallRun(const Vec3f& _offset) { targetOffsetOnWallRun_ = _offset; }

    const Vec3f& getCurrentOffset() const { return currentOffset_; }
    void setCurrentOffset(const Vec3f& offset) { currentOffset_ = offset; }

    const Vec3f& getFirstOffset() const { return firstOffset_; }
    void setFirstOffset(const Vec3f& offset) { firstOffset_ = offset; }

    const Vec3f& getOffsetOnDash() const { return offsetOnDash_; }
    void setOffsetOnDash(const Vec3f& offset) { offsetOnDash_ = offset; }

    const Vec3f& getOffsetOnWallRun() const { return offsetOnWallRun_; }
    void setOffsetOnWallRun(const Vec3f& offset) { offsetOnWallRun_ = offset; }

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
    void setFovYInterpolate(float interpolate) { fovYInterpolate_ = interpolate; }

    float getFixForForwardSpeed() const { return fixForForwardSpeed_; }
    void setFixForForwardSpeed(float speed) { fixForForwardSpeed_ = speed; }
};
