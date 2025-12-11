#pragma once
#include "system/ISystem.h"

/// stl
#include <memory>

/// ECS
// entity
#include <entity/Entity.h>

// system
#include "system/Input/CameraInputSystem.h"
#include "system/Input/PlayerInputSystem.h"

/// <summary>
/// Ghostの入力を更新するシステム
/// </summary>
class GhostInputUpdate
    : public OriGine::ISystem {
public:
    GhostInputUpdate();
    ~GhostInputUpdate() override;

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(OriGine::Entity* _entity);

protected:
    std::unique_ptr<PlayerInputSystem> playerInputSystem_ = nullptr;
    std::unique_ptr<CameraInputSystem> cameraInputSystem_ = nullptr;
};
