#pragma once

#include "system/ISystem.h"

class SubSceneDeactivateByButton
    : public ISystem {
public:
    SubSceneDeactivateByButton();
    ~SubSceneDeactivateByButton() override;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* _entity) override;
};
