#pragma once

/// parent
#include "system/ISystem.h"

/// <summary>
/// カメラの追従処理を行うシステム
/// </summary>
class FollowCameraUpdateSystem
    : public OriGine::ISystem {
public:
    FollowCameraUpdateSystem() : ISystem(OriGine::SystemCategory::Movement) {}
    ~FollowCameraUpdateSystem() = default;
    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(OriGine::EntityHandle _handle) override;
};
