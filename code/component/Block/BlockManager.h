#pragma once

#include "component/IComponent.h"
#include <array>
#include <cstdint>
#include <Vector3.h>

enum class BlockType {
    NORMAL,
    SKULL,
    ADVANTAGE,
    COUNT
};

class BlockManager
    : public IComponent {



private: // variables
    bool isAlive_ = true;
 
    int32_t columNumMax_;
    int32_t HPMax_;

    // size
    Vec2f blockSize_;

    // collision
    float collisionRadius_;

    // pos
    float startPositionX_;
    float nextCreatePositionX_;
    float basePosY_;

    float moveSpeed_;
    float moveIncrementValue_;

     // cost
    std::array<int32_t, static_cast<int32_t>(BlockType::COUNT)> costs_;

public:
    BlockManager() {}
    virtual ~BlockManager() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Save(BinaryWriter& _writer) override;
    void Load(BinaryReader& _reader) override;
    void Finalize() override;

    void CostReset();
    void SpeedIncrementForTime();

public: // accsessor
    /// getter
    int32_t GetColumNumMax() const { return columNumMax_; }
    int32_t GetHpMax() const { return HPMax_; }
    float GetCollisionRadius() const { return collisionRadius_; }
    Vec2f GetBlockSize() const { return blockSize_; }
    float GetStartPositionX() const { return startPositionX_; }
    float GetNextCreatePositionX() const { return nextCreatePositionX_; }
    float GetBasePosY() const { return basePosY_; }
    float GetMoveSpeed() const { return moveSpeed_; }
    float GetMoveIncrementSpeed() const { return moveIncrementValue_; }
    /// setter
  
};
