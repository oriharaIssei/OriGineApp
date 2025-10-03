#pragma once

#include "system/ISystem.h"

class ButtonGroupSystem
    : public ISystem {
public:
    ButtonGroupSystem();
    ~ButtonGroupSystem() override = default;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(GameEntity* _entity) override;
};
