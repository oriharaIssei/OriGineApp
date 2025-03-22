#pragma once

#include "system/ISystem.h"

class CharacterOnCollision
    : public ISystem {
public:
    CharacterOnCollision();
    ~CharacterOnCollision();
    void Initialize()override;
    //void Update() override;
    void Finalize()override;

protected:
    void UpdateEntity(GameEntity* _entity)override;
};
