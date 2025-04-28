// FloorSystem.h
#pragma once
#include "system/ISystem.h"

#include <component/transform/Transform.h>
#include <cstdint>
#include <vector>
#include <Vector3.h>
#include <array>

///====================================================================
// FloorSystem
///====================================================================
class BottomFloorStates;
class FloatingFloorSpawner;
class PillerStates;
class FloorStates;
class FloorModeCreater;
class Audio;

class CreateFloorSystem : public ISystem {
private:
    bool isInited_;

    BottomFloorStates* bottomFloorStates_;
    FloatingFloorSpawner* floatFloorSpawner;
    /*std::vector<FloorModeCreater*> floorModeCreater_;*/
    PillerStates* pillerStates_;
    FloorStates* floorStates_;

    int32_t normalCost_;
    int32_t safeCost_;

    std::array<Audio*, 4> audios_;
    std::array<Audio*, 4> faudios_;

public:
    CreateFloorSystem();
    ~CreateFloorSystem();

    void Initialize() override;
    void Finalize() override;

  
    void CreateFloatingFloor(GameEntity*_entity);


    void CostInit();

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
