#pragma once

#include "system/ISystem.h"

/// <summary>
/// タイトル画面でのプレイヤーの更新を行うシステム
/// プレイヤーのジャンプ入力を無効化し、常に地面に接地している状態にする
/// </summary>
class PlayerUpdateOnTitle
    : public ISystem {
public:
    PlayerUpdateOnTitle();
    ~PlayerUpdateOnTitle() override;
    void Initialize();
    void Finalize();

private:
    void UpdateEntity(Entity* _entity) override;
};
