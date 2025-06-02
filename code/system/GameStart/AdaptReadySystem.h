#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class AdaptReadySystem
    : public ISystem {
private:

    float time_;

public:
    AdaptReadySystem();
    ~AdaptReadySystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;


};
