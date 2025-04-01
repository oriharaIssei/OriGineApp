#pragma once

#include "system/ISystem.h"

class DeletePillerSystem
    : public ISystem {
public:
    DeletePillerSystem();
    ~DeletePillerSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

    
    void AddExplotionEntity(GameEntity* _entity);

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
   
};
