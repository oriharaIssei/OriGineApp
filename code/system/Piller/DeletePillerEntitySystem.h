#pragma once

#include "system/ISystem.h"

class DeletePillerEntitySystem
    : public ISystem {
public:
        DeletePillerEntitySystem();
    ~DeletePillerEntitySystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

   

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
   
};
