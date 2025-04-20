#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class PlayerStates;
class PlayerMoveSystem
    : public ISystem {
private:
   
public:
    PlayerMoveSystem();
    ~PlayerMoveSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;;
    float LerpShortAngle(float a, float b, float t);
   
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
