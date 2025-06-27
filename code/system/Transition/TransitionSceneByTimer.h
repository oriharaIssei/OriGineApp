#pragma once
#include "system/ISystem.h"

class TransitionSceneByTimer
    : public ISystem {
public:
    TransitionSceneByTimer();
    ~TransitionSceneByTimer() override;

    virtual void Initialize() override;
    virtual void Finalize() override;
    // virtual void Update() override;
    // virtual void Edit() override;
private:
    void UpdateEntity(GameEntity* _entity) override;
};
