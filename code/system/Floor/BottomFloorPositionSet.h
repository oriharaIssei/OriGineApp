// FloorSystem.h
#pragma once
#include"system/ISystem.h"

#include<vector>

///====================================================================
// FloorSystem
///====================================================================
class BottomFloorStates;
class FloorAndPillerSpawner;
class PillerStates;
class FloorStates;
class FloorModeCreater;
class BottomFloorPositionSet : public ISystem {
private:
    bool isInited_;

    BottomFloorStates* bottomFloorStates_;
   FloorAndPillerSpawner* floorAndPillerSpawner_;
   std::vector<FloorModeCreater*>floorModeCreater_;
    PillerStates* pillerStates_;
    FloorStates* floorStates_;
  
public:
    BottomFloorPositionSet();
    ~BottomFloorPositionSet();

    void Initialize() override;
    void Finalize() override;

    void CreateBottomFloor();

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
