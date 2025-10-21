#pragma once
#include "system/ISystem.h"

class CreateSpriteFromTimer
    : public ISystem {
public:
    CreateSpriteFromTimer();
    ~CreateSpriteFromTimer() override;
    virtual void Initialize();
    // virtual void Update();
    // virtual void Edit();
    virtual void Finalize();

protected:
    virtual void UpdateEntity(Entity* _entity) override;
};
