#pragma once
#include "system/ISystem.h"

class UpdateCameraForward
    : public ISystem {
public:
    UpdateCameraForward();
    ~UpdateCameraForward();
    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
