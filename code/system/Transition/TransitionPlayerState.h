#pragma once
#include "system/ISystem.h"

class TransitionPlayerState
    : public ISystem {
public:
    TransitionPlayerState() : ISystem(SystemCategory::StateTransition) {};
    ~TransitionPlayerState() = default;

    void Initialize() override{}
    void Finalize() override{}

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
