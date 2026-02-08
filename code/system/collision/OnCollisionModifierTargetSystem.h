#pragma once

#include "system/ISystem.h"

/// <summary>
/// 衝突した際にSpeedModifierのターゲットとして設定するシステム
/// </summary>
class OnCollisionModifierTargetSystem
    : public OriGine::ISystem {
public:
    OnCollisionModifierTargetSystem();
    ~OnCollisionModifierTargetSystem() override;

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Initialize() override;
    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize() override;

private:
    /// <summary>
    /// エンティティの更新処理
    /// </summary>
    void UpdateEntity(OriGine::EntityHandle _handle) override;
};
