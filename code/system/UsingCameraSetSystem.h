#pragma once
#include "system/ISystem.h"

class UsingCameraSetSystem
    : public ISystem {
public:
    UsingCameraSetSystem():ISystem(SystemType::Movement){}

     void Initialize() override;
     void Finalize() override;

protected:
     void UpdateEntity([[maybe_unused]] GameEntity* _entity);
};
