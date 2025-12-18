#pragma once
#include "component/IComponent.h"

/// ECS
// component
#include "component/player/State/PlayerState.h"

/// util
#include "EnumBitMask.h"

/// <summary>
/// 入力をオーバーライドする条件コンポーネント
/// </summary>
struct PlayerStateOverrideCondition
    : public OriGine::IComponent {
    friend void to_json(nlohmann::json& j, const PlayerStateOverrideCondition& c);
    friend void from_json(const nlohmann::json& j, PlayerStateOverrideCondition& c);

public:
    PlayerStateOverrideCondition();
    ~PlayerStateOverrideCondition() override;

    void Initialize(OriGine::Entity* _entity) override;
    void Edit(OriGine::Scene* _scene, OriGine::Entity* _entity, const ::std::string& _parentLabel) override;
    void Finalize() override;

public:
    /// 入力をオーバーライドする条件
    EnumBitmask<PlayerMoveState> overrideConditions;
};
