#pragma once
#include "system/ISystem.h"

class TutorialColliderOnCollision
    : public ISystem {
public:
    TutorialColliderOnCollision();
    ~TutorialColliderOnCollision() override;

    void Initialize() override;
    void Finalize() override;

    void Update() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;

protected:
    GameEntity* playerEntity_         = nullptr;
};
