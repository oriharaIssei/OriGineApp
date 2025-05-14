#pragma once

#include "system/ISystem.h"

class CheckIsUnderPlayer
    : public ISystem {
public:
    CheckIsUnderPlayer();
    ~CheckIsUnderPlayer();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

    
    void ComboCountIncrement();

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
};
