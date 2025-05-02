#pragma once

#include "system/ISystem.h"

class CanageStateFallSystem
    : public ISystem {
public:
    CanageStateFallSystem();
    ~CanageStateFallSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

    
    void ComboCountIncrement();

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
};
