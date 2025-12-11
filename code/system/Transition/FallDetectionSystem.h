#pragma once
#include "system/ISystem.h"

class Stage;
class StageFloor;

#include <Vector3.h>

/// <summary>
/// 落下判定システム
/// </summary>
class FallDetectionSystem
    : public OriGine::ISystem {
public:
    FallDetectionSystem();
    ~FallDetectionSystem() override;

    virtual void Initialize();
    // virtual void Update();
    // virtual void Edit();
    virtual void Finalize();

protected:
    virtual void UpdateEntity(OriGine::Entity* _entity) override;
};
