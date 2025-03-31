#pragma once
// FloorSystem.h
#pragma once
#include "system/ISystem.h"

///====================================================================
// FloorSystem
///====================================================================
class FloorAndPillerSpawner;
class FloorStates;
class PillerStates;
    class CreateFloorAndPillerSystem : public ISystem {
private:
  
public:
    CreateFloorAndPillerSystem();
    ~CreateFloorAndPillerSystem();

    void Initialize() override;
    void Finalize() override;

    void CreateFandP(GameEntity* _entity, FloorAndPillerSpawner* fAndP,
        FloorStates* floorStates, PillerStates* pillerStates);

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
