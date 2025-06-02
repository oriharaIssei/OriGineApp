#pragma once

#include "system/ISystem.h"

class DeleteFloorSystem
    : public ISystem {
public:
    DeleteFloorSystem();
    ~DeleteFloorSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

   

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
   
};
