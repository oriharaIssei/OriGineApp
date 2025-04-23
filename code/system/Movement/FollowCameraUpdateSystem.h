#pragma once

/// parent
#include "system/ISystem.h"

class FollowCameraUpdateSystem
    : public ISystem {
public:
    FollowCameraUpdateSystem() : ISystem(SystemType::Movement) {}
    ~FollowCameraUpdateSystem() = default;
    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
