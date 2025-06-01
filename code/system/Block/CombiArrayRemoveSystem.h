#pragma once


#include "system/ISystem.h"


class CombiArrayRemoveSystem
    : public ISystem {
public:
    CombiArrayRemoveSystem();
    ~CombiArrayRemoveSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;

    private:
  
   
};
