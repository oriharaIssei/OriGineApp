#pragma once

#include "system/ISystem.h"

/// <summary>
/// Buttonの入力状態に応じて色を変更するシステム
/// </summary>
class UpdateButtonColorByState
    : public ISystem {
public:
    UpdateButtonColorByState() : ISystem(SystemCategory::StateTransition) {}
    ~UpdateButtonColorByState() {}

    virtual void Initialize();
    virtual void Finalize();

protected:
    virtual void UpdateEntity(Entity* _entity);
};
