#pragma once
#include "system/ISystem.h"

class Input;

class PlayerInputSystem
    : public ISystem {
public:
    PlayerInputSystem() : ISystem(SystemCategory::Input) {}
    ~PlayerInputSystem() {}

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

private:
    void UpdateEntity(GameEntity* _entity) override;


private:
    Input* input_ = nullptr;
};
