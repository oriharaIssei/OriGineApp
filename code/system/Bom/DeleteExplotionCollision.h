#pragma once

#include "system/ISystem.h"


class DeleteExplotionCollision
    : public ISystem {
public:
    DeleteExplotionCollision();
    ~DeleteExplotionCollision();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

  
protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
   
};
