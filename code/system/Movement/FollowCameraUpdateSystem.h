#pragma once

/// parent
#include "system/ISystem.h"

class FollowCameraUpdateSystem
    : public ISystem {
public:
    FollowCameraUpdateSystem() : ISystem(SystemCategory::Movement) {}
    ~FollowCameraUpdateSystem() = default;
    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* _entity) override;
};
