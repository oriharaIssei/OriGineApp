#pragma once

#include "component/IComponent.h"

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
    IDLE      = 1 << 0, // 待機 (動いていない)
    DASH      = 1 << 1, // ダッシュ(基本移動)
    FALL_DOWN = 1 << 2, // 落下中 (ジャンプ ではない.)
    JUMP      = 1 << 3, // ジャンプ
    WALL_RUN  = 1 << 4, // 壁走り
    WALL_JUMP = 1 << 5, // 壁ジャンプ

    Count = 6
};

enum class PlayerStateFlag {
    NONE       = 0,
    ON_GROUND  = 1 << 0, // 地面に接地している
    ON_WALL    = 1 << 1, // 壁に接触している
    GEAR_UP    = 1 << 2, // ギアアップしている
    IS_GOAL    = 1 << 3, // ゴールした
    IS_PENALTY = 1 << 4, // ペナルティを受けている
    IS_RESTART = 1 << 5, // リスタート中

    Count = 6
};

constexpr int32_t kDefaultPlayerGearLevel = 1; // デフォルトのギアレベル
constexpr int32_t kMaxPlayerGearLevel     = 10; // 最大のギアレベル

/// <summary>
/// プレイヤーの状態を表す変数群
/// </summary>
class PlayerState
    : public IComponent {
    friend void to_json(nlohmann::json& j, const PlayerState& p);
    friend void from_json(const nlohmann::json& j, PlayerState& p);

public:
    PlayerState();
    ~PlayerState() override;

    void Initialize(Entity* _entity) override;
    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
    void Finalize() override;

    /// <summary>
    /// 壁と接触したときの処理
    /// </summary>
    /// <param name="_collisionNormal"></param>
    /// <param name="_entityIndex"></param>
    void OnCollisionWall(const Vec3f& _collisionNormal, int32_t _entityIndex);
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
    int32_t cameraEntityID_ = -1; // カメラのエンティティID
    int32_t dummy           = 0;

    // TransitionPlayerState で更新される
    std::shared_ptr<IPlayerMoveState> moveState_ = nullptr;

    DiffValue<EnumBitmask<PlayerMoveState>> moveStateEnum_;
    DiffValue<EnumBitmask<PlayerStateFlag>> stateFlag_ = EnumBitmask<PlayerStateFlag>(0);

    Vec3f wallCollisionNormal_ = {0.f, 0.f, 0.f};

    int32_t wallEntityIndex_ = -1; // 現在 接触している壁 のエンティティID

    int32_t gearLevel_    = 0;
    float gearUpCoolTime_ = 0.0f;

    float penaltyTime_   = 0.0f; // ペナルティ時間 /sec (制限時間から マイナスする時間)
    float invincibility_ = 0.0f; // ペナルティ無敵時間

public:
    int32_t GetCameraEntityID() const {
        return cameraEntityID_;
    }
    void SetCameraEntityID(int32_t _entityID) {
        cameraEntityID_ = _entityID;
    }

    PlayerMoveState GetStateEnum() const {
        return moveStateEnum_.Current().ToEnum();
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
    int32_t GetWallEntityIndex() const {
        return wallEntityIndex_;
    }

    const Vec3f& GetWallCollisionNormal() const {
        return wallCollisionNormal_;
    }

    bool IsPenalty() const {
        return stateFlag_.Current().HasFlag(PlayerStateFlag::IS_PENALTY);
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

    bool IsRestart() const {
        return stateFlag_.Current().HasFlag(PlayerStateFlag::IS_RESTART);
    }

    bool IsGearUp() const {
        return stateFlag_.Current().HasFlag(PlayerStateFlag::GEAR_UP);
    }
};
