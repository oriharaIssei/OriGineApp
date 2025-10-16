#pragma once
#include "system/ISystem.h"

class ShowGameUIByInputDevice
    : public ISystem {
public:
    ShowGameUIByInputDevice();
    ~ShowGameUIByInputDevice() override = default;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(GameEntity* _entity) override;
};
