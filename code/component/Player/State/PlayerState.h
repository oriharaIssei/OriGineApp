#pragma once

#include "component/IComponent.h"

/// util
#include "util/EnumBitMask.h"

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
    // SLIDE    = 1 << 4, // スライド

    Count = 6 // 5
};

constexpr int32_t kDefaultPlayerGearLevel = 1; // デフォルトのギアレベル
constexpr int32_t kMaxPlayerGearLevel     = 6; // 最大のギアレベル

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

    void OnCollisionWall(const Vec3f& _collisionNormal, int32_t _entityIndex);
    void OffCollisionWall();

    void OnCollisionGround(int32_t _entityIndex);
    void OffCollisionGround();

private:
    EnumBitmask<PlayerMoveState> moveStateEnum_    = PlayerMoveState::IDLE;
    EnumBitmask<PlayerMoveState> preMoveStateEnum_ = PlayerMoveState::IDLE;

    // TransitionPlayerState で更新される
    std::shared_ptr<IPlayerMoveState> moveState_ = nullptr;

    bool onGround_             = false;
    bool isGearUp_             = false;
    bool collisionWithWall_    = false;
    bool isGoal_               = false;
    Vec3f wallCollisionNormal_ = {0.f, 0.f, 0.f};

    int32_t wallEntityIndex_      = -1; // 現在 接触している壁 のエンティティID
    int32_t lastFloorEntityIndex_ = -1; // 最後に接地した床のエンティティID

    int32_t gearLevel_    = 0;
    float gearUpCoolTime_ = 0.0f;

public:
    PlayerMoveState getStateEnum() const {
        return moveStateEnum_.toEnum();
    }
    void setState(const PlayerMoveState& _state) {
        moveStateEnum_ = _state;
    }

    PlayerMoveState getPrevStateEnum() const {
        return preMoveStateEnum_.toEnum();
    }
    void setPrevState(const PlayerMoveState& _prevState) {
        preMoveStateEnum_ = _prevState;
    }

    std::shared_ptr<IPlayerMoveState> getPlayerMoveState() const {
        return moveState_;
    }
    void setPlayerMoveState(std::shared_ptr<IPlayerMoveState> _playerMoveState) {
        moveState_ = _playerMoveState;
    }

    int32_t getGearLevel() const {
        return gearLevel_;
    }
    void setGearLevel(int32_t _gearLevel) {
        gearLevel_ = _gearLevel;
    }

    bool isGoal() const {
        return isGoal_;
    }
    void setGoal(bool _isGoal) {
        isGoal_ = _isGoal;
    }

    bool isOnGround() const {
        return onGround_;
    }
    int32_t getLastFloorEntityIndex() const {
        return lastFloorEntityIndex_;
    }

    bool isCollisionWithWall() const {
        return collisionWithWall_;
    }
    int32_t getWallEntityIndex() const {
        return wallEntityIndex_;
    }

    const Vec3f& getWallCollisionNormal() const {
        return wallCollisionNormal_;
    }

    bool isGearUp() const {
        return isGearUp_;
    }
    void setGearUp(bool _isGearUp) {
        isGearUp_ = _isGearUp;
    }
};
