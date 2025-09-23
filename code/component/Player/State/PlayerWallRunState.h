#pragma once

#include "IPlayerMoveState.h"

class StageWall;
struct Transform;

class PlayerWallRunState
    : public IPlayerMoveState {
public:
    PlayerWallRunState(Scene* _scene, int32_t _playerEntityID) : IPlayerMoveState(_scene, _playerEntityID, PlayerMoveState::DASH) {}
    ~PlayerWallRunState() override {};

    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;

    PlayerMoveState TransitionState() const override;

protected:
    void getWallData(StageWall* _wall);
    // 壁走りの高さ(目標)を計算する
    float calculateWallRunHeight(Transform* _transform);

protected:
    Vec3f prevVelo_            = Vec3f(0.0f, 0.0f, 0.0f); // 前の速度 壁走り前の速度を保存
    float separationGraceTime_ = 0.04f; // オブジェクトが離れていると判定するまでの猶予時間
    float separationLeftTime_  = 0.0f; // 壁との衝突判定の残り時間

    Vec3f wallNormal_   = Vec3f(0.0f, 0.0f, 0.0f); // 壁の法線
    Vec3f wallStartPos_ = Vec3f(0.0f, 0.0f, 0.0f); // 壁の始点
    Vec3f wallEndPos_   = Vec3f(0.0f, 0.0f, 0.0f); // 壁の終点

    float wallRunHeight_ = 1.0f; // 壁走りの高さ
    float currentHeight_ = 0.0f; // 現在の高さ
    float climbTime_     = 0.3f; // 壁を登る時間(currentHeight_がwallRunHeight_になるまでの時間)
    float climbTimer_    = 0.0f; // 壁を登りのタイマー
};
