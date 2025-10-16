#pragma once

#include "system/ISystem.h"

class SubSceneActivateByButton
    : public ISystem {
public:
    SubSceneActivateByButton();
    ~SubSceneActivateByButton() override;
    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(GameEntity* _entity) override;
};
