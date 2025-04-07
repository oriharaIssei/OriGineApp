#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class PlayerStates;
class PlayerMoveSystem
    : public ISystem {
private:
    PlayerStates* entityPlayerStates_=nullptr;
    Transform* transform_             = nullptr;
    Transform* pivotTransform_       = nullptr;

public:
    PlayerMoveSystem();
    ~PlayerMoveSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

    void GetTransformForPlayer(GameEntity* _entity);
  
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
