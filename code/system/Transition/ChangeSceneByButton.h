#pragma once
#include "system/ISystem.h"

/// <summary>
/// SceneManagerによるシーン切り替えをボタン入力で行うシステム
/// </summary>
class ChangeSceneByButton
    : public ISystem {
public:
    ChangeSceneByButton();
    virtual ~ChangeSceneByButton();

    virtual void Initialize() override;
    virtual void Finalize() override;

protected:
    virtual void UpdateEntity(Entity* _entity) override;
};
