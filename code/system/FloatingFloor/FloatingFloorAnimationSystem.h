#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class FloatingFloorAnimationStatus;
class FloatingFloorAnimationSystem
    : public ISystem {
private:
    FloatingFloorAnimationStatus* animationStatus_;
    float time_;

public:
    FloatingFloorAnimationSystem();
    ~FloatingFloorAnimationSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;


};
