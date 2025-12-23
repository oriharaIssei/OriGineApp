#pragma once
#include "system/ISystem.h"

/// <summary>
/// 影面プレーンの位置をプレイヤーの位置に同期させるシステム
/// </summary>
class ShadowPlaneSyncPlayerPosition
    : public OriGine::ISystem {
public:
    ShadowPlaneSyncPlayerPosition();
    ~ShadowPlaneSyncPlayerPosition() override = default;
    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(OriGine::Entity* entity) override;
};
