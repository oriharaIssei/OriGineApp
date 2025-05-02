#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class PlayerStates;
class ScrapFallSystem
    : public ISystem {
private:
   
public:
    ScrapFallSystem();
    ~ScrapFallSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;;
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
