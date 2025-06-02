#pragma once

#include "system/ISystem.h"

class DeleteFloatingFloorSystem
    : public ISystem {
public:
    DeleteFloatingFloorSystem();
    ~DeleteFloatingFloorSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

   

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
   
};
