#pragma once
#include "system/ISystem.h"

/// <summary>
/// Player の状態遷移を管理するシステム
/// </summary>
class TransitionPlayerState
    : public ISystem {
public:
    TransitionPlayerState() : ISystem(SystemCategory::StateTransition) {};
    ~TransitionPlayerState() = default;

    void Initialize() override{}
    void Finalize() override{}

protected:
    void UpdateEntity(Entity* _entity) override;
};
