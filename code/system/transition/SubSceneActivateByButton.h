#pragma once

#include "system/ISystem.h"

/// <summary>
/// ボタン入力でサブシーンを有効化するシステム
/// </summary>
class SubSceneActivateByButton
    : public ISystem {
public:
    SubSceneActivateByButton();
    ~SubSceneActivateByButton() override;
    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* _entity) override;
};
