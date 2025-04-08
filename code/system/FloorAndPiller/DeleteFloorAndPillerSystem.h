#pragma once

#include "system/ISystem.h"

class DeleteFloorAndPillerSystem
    : public ISystem {
public:
        DeleteFloorAndPillerSystem();
    ~DeleteFloorAndPillerSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

   

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
   
};
