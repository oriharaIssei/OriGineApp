#pragma once
#include "system/ISystem.h"

class FallDetectionSystem
    : public ISystem {
public:
    FallDetectionSystem();
    ~FallDetectionSystem() override;

    virtual void Initialize();
    // virtual void Update();
    // virtual void Edit();
    virtual void Finalize();

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
};
