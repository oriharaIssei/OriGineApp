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
public:
    CameraController() {}
    ~CameraController() override {}

    void Initialize(GameEntity* _entity) override;

    bool Edit() override;
    void Save(BinaryWriter& _writer) override;
    void Load(BinaryReader& _reader) override;

    void Finalize() override;

private:
    Transform* followTarget_ = nullptr;

    Vec3f followOffset_             = {0.f, 0.f, 0.f};
    Vec3f interTarget_              = {0.f, 0.f, 0.f};
    Vec2f destinationAngleXY_       = {0.0f, 0.0f};
    float rotateSpeedPadStick_      = 0.0f;
    float rotateSpeedMouse_         = 0.0f;
    float rotateSensitivity_        = 0.0f;
    float interTargetInterpolation_ = 0.0f;

    float maxRotateX_ = 0.0f;
    float minRotateX_ = 0.0f;

public:
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
};
