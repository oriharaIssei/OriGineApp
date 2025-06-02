#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class EffectByBlockAnimationSystem
    : public ISystem {
private:
   
    float time_;
    float scalingElapsed_; 
public:
    EffectByBlockAnimationSystem();
    ~EffectByBlockAnimationSystem();

    void Initialize() override;
    void Finalize() override;

    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
