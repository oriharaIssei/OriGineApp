#pragma once
#include "system/ISystem.h"

class Input;

class PlayerInputSystem
    : public ISystem {
public:
    PlayerInputSystem();
    ~PlayerInputSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;

    Input* input_ = nullptr;
};
