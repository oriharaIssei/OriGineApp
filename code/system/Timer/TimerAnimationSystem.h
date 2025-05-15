#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class TimerAnimationSystem
    : public ISystem {
private:
   
    float time_;
   
public:
    TimerAnimationSystem();
    ~TimerAnimationSystem();

    void Initialize() override;
    void Finalize() override;

    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
