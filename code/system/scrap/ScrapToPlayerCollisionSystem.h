#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class PlayerStates;
class ScrapToPlayerCollisionSystem
    : public ISystem {
private:
   
public:
    ScrapToPlayerCollisionSystem();
    ~ScrapToPlayerCollisionSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;;
   
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
