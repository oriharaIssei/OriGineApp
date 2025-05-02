#pragma once

#include "component/IComponent.h"
#include <array>

class ShockWaveSpawner
    : public IComponent {
public:

    enum class Direction {
        Right,
        Left,
    };

private: // variables
    bool isAlive_ = true;

    float direction_;
    float moveSpeed_;
  
public:
    ShockWaveSpawner() {}
    virtual ~ShockWaveSpawner() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

public: // accsessor
    /// getter
    float GetMoveSpeed() const { return moveSpeed_; }
    float GetDirection() const { return direction_; }
  
    /// setter
    void SetDirection(const Direction& _direction);
};
