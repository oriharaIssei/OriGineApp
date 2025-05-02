#pragma once

#include "component/IComponent.h"
#include <array>

class ShockWaveSpawner
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const ShockWaveSpawner& _block);
    friend void from_json(const nlohmann::json& _json, ShockWaveSpawner& _block);

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

    virtual void Finalize();

public: // accsessor
    /// getter
    float GetMoveSpeed() const { return moveSpeed_; }
    float GetDirection() const { return direction_; }

    /// setter
    void SetDirection(const Direction& _direction);
};
