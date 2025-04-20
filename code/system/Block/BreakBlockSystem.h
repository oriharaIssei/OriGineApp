#pragma once

#include "system/ISystem.h"

class BreakBlockSystem
    : public ISystem {
public:
    BreakBlockSystem();
    ~BreakBlockSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

   

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
   
};
