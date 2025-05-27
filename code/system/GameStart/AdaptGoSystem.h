#pragma once
#include "component/transform/Transform.h"
#include "system/ISystem.h"

class AdaptGoSystem
    : public ISystem {
private:
    float time_;

public:
    AdaptGoSystem();
    ~AdaptGoSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

    void ComboReset();

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
