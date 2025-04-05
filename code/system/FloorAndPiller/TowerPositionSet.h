// FloorSystem.h
#pragma once
#include "system/ISystem.h"

#include <component/transform/Transform.h>
#include <cstdint>
#include <vector>

///====================================================================
// FloorSystem
///====================================================================
class BottomFloorStates;
class FloorAndPillerSpawner;
class PillerStates;
class FloorStates;
class FloorModeCreater;

class TowerPositionSet : public ISystem {
private:
    bool isInited_;

    BottomFloorStates* bottomFloorStates_;
    FloorAndPillerSpawner* floorAndPillerSpawner_;
    std::vector<FloorModeCreater*> floorModeCreater_;
    PillerStates* pillerStates_;
    FloorStates* floorStates_;

    int32_t normalCost_;
    int32_t safeCost_;

public:
    TowerPositionSet();
    ~TowerPositionSet();

    void Initialize() override;
    void Finalize() override;

    void CreateBottomFloor();
    void CreateTower();

    void SetPivotQuaternion(Transform* pivotTransform, const int32_t& index);
    void SetQuaternion(Transform* pivotTransform, Transform* Transform);

    void CostInit();

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
