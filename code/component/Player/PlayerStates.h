#pragma once

#include "component/IComponent.h"
#include <cstdint>

class PlayerStates
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    float moveSpeed_;       // 移動速度
    float moveRadius_;      // 移動半径
    float theta_;           // 現在の移動角度
    int32_t settingBomNum_; // 設置爆弾数

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
    /// setter
    void SetIncrementTheta(float _theta) { theta_ += _theta; }
    void SetTheta(float _theta) { theta_ = _theta; }
};
