#pragma once
#include "system/ISystem.h"

/// <summary>
/// CameraMotionBob でカメラを揺らすシステム
/// </summary>
class CameraMotionBobSystem
    : public ISystem {
public:
    CameraMotionBobSystem();
    ~CameraMotionBobSystem() override = default;

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* _entity) override;
};
