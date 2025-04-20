#pragma once

#include "system/ISystem.h"

class DeleteBlockSystem
    : public ISystem {
public:
    DeleteBlockSystem();
    ~DeleteBlockSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

   

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
   
};
