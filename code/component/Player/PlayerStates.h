#pragma once

#include "component/IComponent.h"
#include <component/transform/Transform.h>
#include <cstdint>
#include <Entity.h>
#include <Vector3.h>
#include <Quaternion.h>

class PlayerStates
    : public IComponent {
    friend void to_json(nlohmann::json& j, const PlayerStates& p);
    friend void from_json(const nlohmann::json& j, PlayerStates& p);

private: // variables
    bool isAlive_        = true;
    bool isBigBomHaving_ = false;

    float moveSpeed_; // 移動速度
    float direction_; // 移動方向
    float offSetY_;

    Vec3f followCameraOffset_;

    int32_t bigBomPointMax_;
    int32_t bomExplotionNum_; // 爆発数
    int32_t currentBigBomPoint_ = 0;

    Transform* pivotTransform_ = nullptr; // 回転の中心となるオブジェクト
    Transform* transform_      = nullptr; // 自身のTransform

   Quaternion partsHeadRotate_;

public:
    PlayerStates() {}
    virtual ~PlayerStates() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    void IncrementCurrentBigBomPoint();
    void ReSetCurrentBigBomPoint();

public: // accsessor
    /// getter
    float GetMoveSpeed() const { return moveSpeed_; }
    float GetOffSetY() const { return offSetY_; }
    float GetDirection() const { return direction_; }
    Transform* GetPivotTransform() const { return pivotTransform_; }
    Transform* GetTransform() const { return transform_; }
    int32_t GetBomExplotionNum() const { return bomExplotionNum_; }
    int32_t GetBigBomPointMax() const { return bigBomPointMax_; }
    int32_t GetCurrentBigBomPoint() const { return currentBigBomPoint_; }
    Vec3f GetFollowOffset() const { return followCameraOffset_; }
    bool GetIsBigBomHaving() const { return isBigBomHaving_; }
    Quaternion GetPartsHeadRotate() const { return partsHeadRotate_; }
    /// setter

    void SetDirection(float _direction) { direction_ = _direction; }
    void SetTransform(Transform* _transform) { transform_ = _transform; }
    void SetPivotTransform(Transform* _pivotTransform) { pivotTransform_ = _pivotTransform; }
    void SetincrementBomExplotionNum() { bomExplotionNum_++; }
    void SetIsBigBomHaving(const bool& is) { isBigBomHaving_ = is; }
    void SetPartsHeadRotate(const Quaternion& q) { partsHeadRotate_ = q; }
};
