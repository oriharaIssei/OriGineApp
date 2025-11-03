#pragma once

#include "system/ISystem.h"

/// <summary>
/// サブシーンをボタンで非アクティブ化するシステム
/// </summary>
/// <remarks>
/// このシステムが動作するシーンがサブシーンであることが前提
/// </remarks>
class SubSceneDeactivateByButton
    : public ISystem {
public:
    SubSceneDeactivateByButton();
    ~SubSceneDeactivateByButton() override;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* _entity) override;
};
