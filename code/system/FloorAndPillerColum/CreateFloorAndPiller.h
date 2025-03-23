#pragma once
// FloorSystem.h
#pragma once
#include "system/ISystem.h"

///====================================================================
// FloorSystem
///====================================================================
class CreateFloorAndPiller : public ISystem {
public:
    CreateFloorAndPiller();
    ~CreateFloorAndPiller();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
