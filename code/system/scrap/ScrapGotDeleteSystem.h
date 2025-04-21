#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class PlayerStates;
class ScrapGotDeleteSystem
    : public ISystem {
private:
   
public:
    ScrapGotDeleteSystem();
    ~ScrapGotDeleteSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;;
   
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
