#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class PlayerAnimationSystem
    : public ISystem {
private:

    float time_;

public:
    PlayerAnimationSystem();
    ~PlayerAnimationSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;


};
