#pragma once
#include "system/ISystem.h"

/// <summary>
/// Buttonによるアプリケーション終了システム
/// </summary>
class ExitApplicationByButton
    : public ISystem {
public:
    ExitApplicationByButton();
    ~ExitApplicationByButton() override;

    void Initialize() override {}
    void Finalize() override {}

private:
    void UpdateEntity(Entity* _entity) override;
};
