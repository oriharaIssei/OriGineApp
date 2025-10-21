#pragma once
#include "system/ISystem.h"

class SceneManager;

class ChangeSceneByButton
    : public ISystem {
public:
    ChangeSceneByButton();
    virtual ~ChangeSceneByButton();

    virtual void Initialize() override;
    virtual void Finalize() override;

protected:
    virtual void UpdateEntity(Entity* _entity) override;

private:
    SceneManager* sceneManager_ = nullptr;
};
