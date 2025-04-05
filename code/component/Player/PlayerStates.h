#pragma once

#include "component/IComponent.h"
#include <cstdint>
#include <Entity.h>
#include <component/transform/Transform.h>

class PlayerStates
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    float moveSpeed_;       // 移動速度
    float moveRadius_;      // 移動半径
    float theta_;           // 現在の移動角度
    float direction_;       // 移動方向
    float offSetY_;
   
    Transform* pivotTransform_ = nullptr; // 回転の中心となるオブジェクト
    Transform* transform_      = nullptr; // 自身のTransform

    int32_t bomExplotionNum_; // 爆発数

public:
    PlayerStates() {}
    virtual ~PlayerStates() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

public: // accsessor
    /// getter
    float GetMoveSpeed() const { return moveSpeed_; }
    float GetTheta() const { return theta_; }
    float GetMoveRadius() const { return moveRadius_; }
    float GetOffSetY() const { return offSetY_; }
    float GetDirection() const { return direction_; }
    Transform* GetPivotTransform() const { return pivotTransform_; }
    Transform* GetTransform() const { return transform_; }
    int32_t GetBomExplotionNum() const { return bomExplotionNum_; }
    /// setter
    void SetIncrementTheta(float _theta) { theta_ += _theta; }
    void SetTheta(float _theta) { theta_ = _theta; }
    void SetDirection(float _direction) { direction_ = _direction; }
    void SetTransform(Transform* _transform) { transform_ = _transform; }
    void SetPivotTransform(Transform* _pivotTransform) { pivotTransform_ = _pivotTransform; }
    void SetincrementBomExplotionNum() { bomExplotionNum_++; }
};
