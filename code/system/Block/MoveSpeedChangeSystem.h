#pragma once

#include "system/ISystem.h"

class MoveSpeedChangeSystem
    : public ISystem {
public:
    MoveSpeedChangeSystem();
    ~MoveSpeedChangeSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

   

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
   
};
