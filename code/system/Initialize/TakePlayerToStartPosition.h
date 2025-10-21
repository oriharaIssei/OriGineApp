#pragma once
#include "system/ISystem.h"
class TakePlayerToStartPosition
    : public ISystem {
public:
    TakePlayerToStartPosition();
    ~TakePlayerToStartPosition() override;
    virtual void Initialize();
    // virtual void Update();
    // virtual void Edit();
    virtual void Finalize();

protected:
    virtual void UpdateEntity(Entity* _entity) override;

};
