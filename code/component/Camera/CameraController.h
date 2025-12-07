#pragma once

/// parent
#include "component/IComponent.h"

/// engine
// component
#include "component/transform/Transform.h"

/// math
#include "math/Vector2.h"
#include "math/Vector3.h"

/// <summary>
/// カメラの挙動を制御するためのデータ
/// </summary>
class CameraController
    : public IComponent {
    friend void to_json(nlohmann::json& j, const CameraController& _cameraController);
    friend void from_json(const nlohmann::json& j, CameraController& _cameraController);

public:
    CameraController() {}
    ~CameraController() override {}

    void Initialize(Entity* _entity) override;

    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;

    void Finalize() override;

    /// <summary>
    /// PlayerのギアレベルからFovYを計算する
    /// </summary>
    /// <param name="_level"></param>
    /// <returns></returns>
    float CalculateFovYByPlayerGearLevel(int32_t _level) const;

private:
    Transform* followTarget_ = nullptr;

    Vec3f forward_           = {0.f, 0.f, 1.f}; // カメラの前方向
    float angleLimitY_       = 0.0f;
    bool isAutoLookAtPlayer_ = true; // 自動でプレイヤーを注視するかどうか

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
    bool GetIsAutoLookAtPlayer() const { return isAutoLookAtPlayer_; }
    void SetIsAutoLookAtPlayer(bool isAutoLookAtPlayer) { isAutoLookAtPlayer_ = isAutoLookAtPlayer; }

    const Vec3f& GetForward() const { return forward_; }
    void SetForward(const Vec3f& forward) { forward_ = forward; }

    float GetAngleLimitY() const { return angleLimitY_; }
    void SetAngleLimitY(float angleLimitY) { angleLimitY_ = angleLimitY; }

    const Vec3f& GetCurrentTargetOffset() const { return currentTargetOffset_; }
    void SetCurrentTargetOffset(const Vec3f& _offset) { currentTargetOffset_ = _offset; }

    const Vec3f& GetFirstTargetOffset() const { return firstTargetOffset_; }
    void SetFirstTargetOffset(const Vec3f& offset) { firstTargetOffset_ = offset; }

    const Vec3f& GetTargetOffsetOnDash() const { return targetOffsetOnDash_; }
    void SetTargetOffsetOnDash(const Vec3f& _offset) { targetOffsetOnDash_ = _offset; }

    const Vec3f& GetTargetOffsetOnWallRun() const { return targetOffsetOnWallRun_; }
    void SetTargetOffsetOnWallRun(const Vec3f& _offset) { targetOffsetOnWallRun_ = _offset; }

    const Vec3f& GetCurrentOffset() const { return currentOffset_; }
    void SetCurrentOffset(const Vec3f& offset) { currentOffset_ = offset; }

    const Vec3f& GetFirstOffset() const { return firstOffset_; }
    void SetFirstOffset(const Vec3f& offset) { firstOffset_ = offset; }

    const Vec3f& GetOffsetOnDash() const { return offsetOnDash_; }
    void SetOffsetOnDash(const Vec3f& offset) { offsetOnDash_ = offset; }

    const Vec3f& GetOffsetOnWallRun() const { return offsetOnWallRun_; }
    void SetOffsetOnWallRun(const Vec3f& offset) { offsetOnWallRun_ = offset; }

    const Vec3f& GetInterTarget() const { return interTarget_; }
    void SetInterTarget(const Vec3f& target) { interTarget_ = target; }

    const Vec2f& GetDestinationAngleXY() const { return destinationAngleXY_; }
    void SetDestinationAngleXY(const Vec2f& angle) { destinationAngleXY_ = angle; }

    float GetRotateSpeedMouse() const { return rotateSpeedMouse_; }
    void SetRotateSpeedMouse(float speed) { rotateSpeedMouse_ = speed; }

    float GetRotateSpeedPadStick() const { return rotateSpeedPadStick_; }
    void SetRotateSpeedPadStick(float speed) { rotateSpeedPadStick_ = speed; }

    float GetRotateSensitivity() const { return rotateSensitivity_; }
    void SetRotateSensitivity(float sensitivity) { rotateSensitivity_ = sensitivity; }

    float GetInterTargetInterpolation() const { return interTargetInterpolation_; }
    void SetInterTargetInterpolation(float interpolation) { interTargetInterpolation_ = interpolation; }

    float GetMaxRotateX() const { return maxRotateX_; }
    void SetMaxRotateX(float maxRotateX) { maxRotateX_ = maxRotateX; }

    float GetMinRotateX() const { return minRotateX_; }
    void SetMinRotateX(float minRotateX) { minRotateX_ = minRotateX; }

    const Transform* GetFollowTarget() const { return followTarget_; }
    void SetFollowTarget(Transform* target) { followTarget_ = target; }

    float GetFovYInterpolate() const { return fovYInterpolate_; }
    void SetFovYInterpolate(float interpolate) { fovYInterpolate_ = interpolate; }

    float GetFixForForwardSpeed() const { return fixForForwardSpeed_; }
    void SetFixForForwardSpeed(float speed) { fixForForwardSpeed_ = speed; }
};
