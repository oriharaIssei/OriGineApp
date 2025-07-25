#pragma once
#include "system/ISystem.h"

class PlayerMoveSystem
    : public ISystem {
public:
    PlayerMoveSystem() : ISystem(SystemType::Movement) {}
    ~PlayerMoveSystem() {}

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

private:
    void UpdateEntity(GameEntity* _entity) override;
};
