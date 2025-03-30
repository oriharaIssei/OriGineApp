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
    bool isCreated_;

public:
    CreateFloorAndPillerSystem();
    ~CreateFloorAndPillerSystem();

    void Initialize() override;
    void Finalize() override;

    void CreateFandP(GameEntity* _entity,const FloorAndPillerSpawner& fAndP,
        FloorStates* floorStates, PillerStates* pillerStates);

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
