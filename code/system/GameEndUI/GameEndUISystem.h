#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class GameEndUISystem
    : public ISystem {
private:
   
    float time_;
   
public:
    GameEndUISystem();
    ~GameEndUISystem();

    void Initialize() override;
    void Finalize() override;

    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
