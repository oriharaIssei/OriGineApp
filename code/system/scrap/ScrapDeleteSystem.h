#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class PlayerStates;
class ScrapDeleteSystem
    : public ISystem {
private:
   
public:
    ScrapDeleteSystem();
    ~ScrapDeleteSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;;
   
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
