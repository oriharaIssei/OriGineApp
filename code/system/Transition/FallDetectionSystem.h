#pragma once
#include "system/ISystem.h"

class Stage;
class StageFloor;

#include <Vector3.h>

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
    virtual void UpdateEntity(Entity* _entity) override;

    Vec3f RetryPos(Stage* _stage, StageFloor* _stageFloor, const Vec3f& _currentPos);
};
