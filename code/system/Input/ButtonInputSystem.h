#pragma once

#include "system/ISystem.h"

class Input;
class ButtonInputSystem
    : public ISystem {
public:
    ButtonInputSystem() : ISystem(SystemCategory::Input) {}
    ~ButtonInputSystem() {}

    virtual void Initialize();
    virtual void Finalize();

protected:
    virtual void UpdateEntity(GameEntity* _entity);

private:
    Input* input_ = nullptr;
};
