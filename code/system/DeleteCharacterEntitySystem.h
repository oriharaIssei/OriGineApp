#pragma once
#include "system/ISystem.h"

class DeleteCharacterEntitySystem
    : public ISystem {
public:
    DeleteCharacterEntitySystem();
    ~DeleteCharacterEntitySystem();

    void Initialize() {}
    // void Update();
    void Finalize() {}

protected:
    void UpdateEntity(GameEntity* _entity);
};
