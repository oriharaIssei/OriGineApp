#pragma once
#include "system/ISystem.h"
#include <entity/Entity.h>

/// ECS
// component
struct GhostReplayComponent;

/// <summary>
/// PlayRecorderの初期化
/// </summary>
class GhostInitializeSystem
    : public OriGine::ISystem {
public:
    GhostInitializeSystem();
    ~GhostInitializeSystem() override;

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(OriGine::Entity* _entity) override;
    bool InitializeGhostReplayComponent(GhostReplayComponent* _comp);
};
