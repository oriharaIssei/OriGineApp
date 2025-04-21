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
    float startPositionZ_;
    float nextCreatePositionX_;
    float deadPositionX_;
    float basePosY_;

    // speed
    float moveSpeed_;
    std::array<float, 6> moveSpeeds_;

    // randomCreate
    std::array<int32_t, static_cast<int32_t>(BlockType::COUNT)> randomPar_;
    std::array<int32_t, static_cast<int32_t>(BlockType::COUNT)> costs_;
    std::array<int32_t, static_cast<int32_t>(BlockType::COUNT)> currentCosts_;
    std::array<int32_t, static_cast<int32_t>(BlockType::COUNT)> generateInterval_{}; // 各BlockTypeの生成間隔（列ごと）
    std::array<int32_t, static_cast<int32_t>(BlockType::COUNT)> lineCounter_{};      // 現在の行数カウント


public:
    BlockManager() {}
    virtual ~BlockManager() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Save(BinaryWriter& _writer) override;
    void Load(BinaryReader& _reader) override;
    void Finalize() override;

    void CostReset();
    void ResetLineCounter(BlockType type);
    void SpeedChangeForTime(const float& time);

public: // accsessor
    /// getter
    int32_t GetColumNumMax() const { return columNumMax_; }
    int32_t GetHpMax() const { return HPMax_; }
    float GetCollisionRadius() const { return collisionRadius_; }
    Vec2f GetBlockSize() const { return blockSize_; }
    float GetStartPositionX() const { return startPositionX_; }
    float GetStartPositionZ() const { return startPositionZ_; }
    float GetNextCreatePositionX() const { return nextCreatePositionX_; }
    float GetBasePosY() const { return basePosY_; }
    float GetMoveSpeed() const { return moveSpeed_; }
    float GetDeadPosition() const { return deadPositionX_; }
    int32_t GetRandomPar(BlockType type) const { return randomPar_[static_cast<int32_t>(type)]; }
    int32_t GetCost(BlockType type) const { return costs_[static_cast<int32_t>(type)]; }
    int32_t GetCurrentCost(BlockType type) const { return currentCosts_[static_cast<int32_t>(type)]; }
    int32_t GetGenerateInterval(BlockType type) const { return generateInterval_[static_cast<int32_t>(type)]; }
    int32_t GetLineCounter(BlockType type) const { return lineCounter_[static_cast<int32_t>(type)]; }
   

    /// setter
    void SetCurrentCostIncrement(BlockType type)  {  currentCosts_[static_cast<int32_t>(type)]++; }
    void SetIncrementLineCounter(BlockType type) { lineCounter_[static_cast<int32_t>(type)]++; }
};
