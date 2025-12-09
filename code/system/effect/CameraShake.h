#pragma once
#include "system/ISystem.h"

/// <summary>
/// CameraShakeSourceComponentで カメラシェイクを制御するシステム
/// </summary>
class CameraShake
    : public ISystem {
public:
    CameraShake();
    ~CameraShake() override;

    void Initialize() override;
    void Finalize();

private:
    void UpdateEntity(Entity* _entity) override;
};
