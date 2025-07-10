#pragma once
#include "system/ISystem.h"

class ChangeSceneByButton
    : public ISystem {
public:
    ChangeSceneByButton();
    virtual ~ChangeSceneByButton();

    virtual void Initialize() override;
    virtual void Finalize() override;

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;

private:
};
