#pragma once

#include "system/ISystem.h"


class DeleteBigExplotionCollision
    : public ISystem {
public:
    DeleteBigExplotionCollision();
    ~DeleteBigExplotionCollision();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

  
protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
   
};
