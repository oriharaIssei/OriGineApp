#pragma once

#include "component/IComponent.h"

/// stl
#include <memory>

/// util
#include "util/DiffValue.h"
#include "util/EnumBitmask.h"

/// math
#include <math/Vector3.h>

class IPlayerMoveState;

/// <summary>
/// プレイヤーの 移動状態 を表す列挙体
/// </summary>
enum class PlayerMoveState {
    IDLE        = 1 << 0, // 待機 (動いていない)
    DASH        = 1 << 1, // ダッシュ(基本移動)
    FALL_DOWN   = 1 << 2, // 落下中 (ジャンプ ではない.)
    JUMP        = 1 << 3, // ジャンプ
    WALL_RUN    = 1 << 4, // 壁走り
    WALL_JUMP   = 1 << 5, // 壁ジャンプ
    WHEELIE_RUN = 1 << 6, // ウィリー走行

    Count = 7
};
static std::map<PlayerMoveState, const char*> moveStateName = {
    {PlayerMoveState::IDLE, "IDLE"},
    {PlayerMoveState::DASH, "DASH"},
    {PlayerMoveState::FALL_DOWN, "FALL_DOWN"},
    {PlayerMoveState::JUMP, "JUMP"},
    {PlayerMoveState::WALL_RUN, "WALL_RUN"},
    {PlayerMoveState::WALL_JUMP, "WALL_JUMP"},
    {PlayerMoveState::WHEELIE_RUN, "WHEELIE_RUN"},
};

enum class PlayerStateFlag {
    NONE       = 0,
    ON_GROUND  = 1 << 0, // 地面に接地している
    ON_WALL    = 1 << 1, // 壁に接触している
    WHEELIE    = 1 << 2, // ウィリーしている
    GEAR_UP    = 1 << 3, // ギアアップしている
    IS_GOAL    = 1 << 4, // ゴールした
    IS_PENALTY = 1 << 5, // ペナルティを受けている
    IS_RESTART = 1 << 6, // リスタート中

    Count = 7
};

constexpr int32_t kDefaultPlayerGearLevel = 1; // デフォルトのギアレベル
constexpr int32_t kMaxPlayerGearLevel     = 6; // 最大のギアレベル

/// <summary>
/// プレイヤーの状態を表す変数群
/// </summary>
class PlayerState
    : public OriGine::IComponent {
    friend void to_json(nlohmann::json& j, const PlayerState& p);
    friend void from_json(const nlohmann::json& j, PlayerState& p);

public:
    PlayerState();
    ~PlayerState() override;

    void Initialize(OriGine::Scene* _scene, OriGine::EntityHandle _owner) override;
    void Edit(OriGine::Scene* _scene, OriGine::EntityHandle _owner, const std::string& _parentLabel) override;
    void Finalize() override;

    /// <summary>
    /// 壁と接触したときの処理
    /// </summary>
    /// <param name="_collisionNormal"></param>
    /// <param name="_entityHandle"></param>
    void OnCollisionWall(const OriGine::Vec3f& _collisionNormal, OriGine::EntityHandle _entityHandle, bool _isWheelie = false);
    /// <summary>
    /// 壁との接触がなくなったときの処理
    /// </summary>
    void OffCollisionWall();

    /// <summary>
    /// 地面と接触したときの処理
    /// </summary>
    void OnCollisionGround();
    /// <summary>
    /// 地面との接触がなくなったときの処理
    /// </summary>
    void OffCollisionGround();
    /// <summary>
    /// 障害物と接触したときの処理
    /// </summary>
    /// <param name="_penaltyTime">ペナルティー時間</param>
    /// <param name="_invincibility"></param>
    void OnCollisionObstacle(float _penaltyTime, float _invincibility);
    /// <summary>
    /// ペナルティを受ける
    /// </summary>
    /// <returns>ペナルティー時間</returns>
    float SufferPenalty();

private:
    OriGine::EntityHandle followCameraEntityHandle_ = OriGine::EntityHandle(); // カメラのエンティティID

    // TransitionPlayerState で更新される
    std::shared_ptr<IPlayerMoveState> moveState_ = nullptr;

    DiffValue<EnumBitmask<PlayerMoveState>> moveStateEnum_;
    DiffValue<EnumBitmask<PlayerStateFlag>> stateFlag_ = EnumBitmask<PlayerStateFlag>(0);

    OriGine::Vec3f wallCollisionNormal_ = {0.f, 0.f, 0.f};

    OriGine::EntityHandle wallEntityHandle_ = OriGine::EntityHandle(); // 現在 接触している壁 のエンティティID

    int32_t gearLevel_    = 0;
    float gearUpCoolTime_ = 0.0f;

    float penaltyTime_   = 0.0f; // ペナルティ時間 /sec (制限時間から マイナスする時間)
    float invincibility_ = 0.0f; // ペナルティ無敵時間

public:
    OriGine::EntityHandle GetCameraEntityHandle() const {
        return followCameraEntityHandle_;
    }
    void SetCameraEntityHandle(OriGine::EntityHandle _handle) {
        followCameraEntityHandle_ = _handle;
    }

    PlayerMoveState GetStateEnum() const {
        return moveStateEnum_.Current().ToEnum();
    }
    PlayerMoveState GetPreStateEnum() const {
        return moveStateEnum_.Prev().ToEnum();
    }

    DiffValue<EnumBitmask<PlayerMoveState>>& GetStateEnumRef() {
        return moveStateEnum_;
    }

    std::shared_ptr<IPlayerMoveState> GetPlayerMoveState() const {
        return moveState_;
    }
    void SetPlayerMoveState(std::shared_ptr<IPlayerMoveState> _playerMoveState);

    PlayerStateFlag GetStateFlag() const {
        return stateFlag_.Current().ToEnum();
    }
    DiffValue<EnumBitmask<PlayerStateFlag>>& GetStateFlagRef() {
        return stateFlag_;
    }

    int32_t GetGearLevel() const {
        return gearLevel_;
    }
    void SetGearLevel(int32_t _gearLevel) {
        gearLevel_ = _gearLevel;
    }

    bool IsGoal() const {
        return stateFlag_.Current().HasFlag(PlayerStateFlag::IS_GOAL);
    }

    bool IsOnGround() const {
        return stateFlag_.Current().HasFlag(PlayerStateFlag::ON_GROUND);
    }

    bool IsCollisionWithWall() const {
        return stateFlag_.Current().HasFlag(PlayerStateFlag::ON_WALL);
    }
    OriGine::EntityHandle GetWallEntityIndex() const {
        return wallEntityHandle_;
    }

    const OriGine::Vec3f& GetWallCollisionNormal() const {
        return wallCollisionNormal_;
    }

    float GetPenaltyTime() const {
        return penaltyTime_;
    }
    void SetPenaltyTime(float _time) {
        penaltyTime_ = _time;
    }
    void SubtractPenaltyTime(float _time) {
        penaltyTime_ -= _time;
        if (penaltyTime_ < 0.0f) {
            penaltyTime_ = 0.0f;
        }
    }
    float GetInvincibilityTime() const {
        return invincibility_;
    }
    void SetInvincibilityTime(float _time) {
        invincibility_ = _time;
    }
    void SubtractInvincibilityTime(float _time) {
        invincibility_ -= _time;
        if (invincibility_ < 0.0f) {
            invincibility_ = 0.0f;
        }
    }

    bool IsPenalty() const {
        return stateFlag_.Current().HasFlag(PlayerStateFlag::IS_PENALTY);
    }

    bool IsWheelie() const {
        return stateFlag_.Current().HasFlag(PlayerStateFlag::WHEELIE);
    }

    bool IsRestart() const {
        return stateFlag_.Current().HasFlag(PlayerStateFlag::IS_RESTART);
    }

    bool IsGearUp() const {
        return stateFlag_.Current().HasFlag(PlayerStateFlag::GEAR_UP);
    }
};
