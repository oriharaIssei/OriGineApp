#pragma once
#include "system/ISystem.h"

/// stl
#include <memory>

/// system
#include "system/initialize/CreateSpriteFromTimer.h"

/// math
#include "math/Matrix4x4.h"

class PenaltySystem
    : public ISystem {
public:
    PenaltySystem();
    ~PenaltySystem();
    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* _entity) override;

protected:
    std::unique_ptr<CreateSpriteFromTimer> createSpriteFromTimerSystem_ = nullptr;
    Matrix4x4 viewPortMat_; // ビューポート行列
};
