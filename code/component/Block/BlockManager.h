#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <array>
#include <cstdint>
#include <Vector3.h>

constexpr int LEVEL_MAX = 6;

enum class BlockType {
    NORMAL,
    ADVANTAGE,
    SKULL,
    COUNT
};

enum class EaseType {
    NONE,
    SCALING,
    MOVESCALING,
};

class LevelUIParentStatus;
class BlockManager
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const BlockManager& _blockManager);
    friend void from_json(const nlohmann::json& _json, BlockManager& _blockManager);

public:
    struct BlockRandomParams {
        std::array<int32_t, static_cast<int32_t>(BlockType::COUNT)> randomPar{};
        std::array<int32_t, static_cast<int32_t>(BlockType::COUNT)> costs{};
        std::array<int32_t, static_cast<int32_t>(BlockType::COUNT)> generateInterval{};
        std::array<int32_t, static_cast<int32_t>(BlockType::COUNT)> randomParRightofAdvance{};
        std::array<int32_t, static_cast<int32_t>(BlockType::COUNT)> randomParUPValue{};
    };

private: // variables
    bool isAlive_ = true;

    int32_t columnMax_;
    int32_t rowMax_;
    int32_t HPMax_;

    // size
    Vec3f blockSize_;
    Vec3f scalingSize_;

    // result
    Vec3f resultScale_;

    // collision
    float collisionRadius_;

    // pos
    float startPositionX_;
    float startPositionZ_;
    float nextCreatePositionX_;
    float deadPositionX_;
    float basePosY_;

    // speed
    float moveTenpo_;
    int32_t moveTenpoNum_;
    std::array<float, LEVEL_MAX> moveTenpos_;

    // reaction
    Easing scalingEase_;
    Easing moveScalingEase_;
    EaseType easeType_ = EaseType::SCALING;
    float moveTimemax_;

    //
    bool isMove_ = false;

    // level
    std::array<float, LEVEL_MAX> nextLevelTime_;
    int32_t currentLevel_ = 0;
    std::array<BlockRandomParams, LEVEL_MAX> levelParams_;
    // randomCreate

    std::array<int32_t, static_cast<int32_t>(BlockType::COUNT)> currentCosts_;
    std::array<int32_t, static_cast<int32_t>(BlockType::COUNT)> lineCounter_; // 現在の行数カウント
    std::array<float, static_cast<int32_t>(BlockType::COUNT)> scoreValue_{}; // 現在の行数カウント

    //
    BlockRandomParams blockRandomParms_;

    /// ZApear
    Easing breakApearEasing_;
    Easing breakBackEasing_;
    float startZPos_;
    float endZPos_;

public:
    BlockManager() {}
    virtual ~BlockManager() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Finalize() override;

    void CostReset();
    void ResetLineCounter(BlockType type);
    void ChangeNextLevel(float& time, LevelUIParentStatus* levelUI);

    void ScalingEaseUpdate(const float& t);
    void ResetScalingEase();
    void SetMoveTempoForLevel();

    void LineIncrement();

    const char* ToStringByBlockType(BlockType type);

    void ApplyLevelParams(int32_t level);

public: // accsessor
    /// getter

    Easing GetBreakApearEasing() const { return breakApearEasing_; }
    Easing GetBreakBackEasing() const { return breakBackEasing_; }
    float GetStartZPos() const { return startZPos_; }
    float GetEndZPos() const { return endZPos_; }
    int32_t GetColumnMaxNum() const { return columnMax_; }
    int32_t GetRowMaxNum() const { return rowMax_; }
    int32_t GetHpMax() const { return HPMax_; }
    float GetCollisionRadius() const { return collisionRadius_; }
    Vec3f GetBlockSize() const { return blockSize_; }
    Vec3f GetScalingSize() const { return scalingSize_; }
    float GetStartPositionX() const { return startPositionX_; }
    float GetStartPositionZ() const { return startPositionZ_; }
    float GetNextCreatePositionX() const { return nextCreatePositionX_; }
    float GetBasePosY() const { return basePosY_; }
    float GetMoveTenpo() const { return moveTenpo_; }
    float GetDeadPosition() const { return deadPositionX_; }
    int32_t GetRandomPar(BlockType type) const { return blockRandomParms_.randomPar[static_cast<int32_t>(type)]; }
    int32_t GetCost(BlockType type) const { return blockRandomParms_.costs[static_cast<int32_t>(type)]; }
    int32_t GetCurrentCost(BlockType type) const { return currentCosts_[static_cast<int32_t>(type)]; }
    int32_t GetGenerateInterval(BlockType type) const { return blockRandomParms_.generateInterval[static_cast<int32_t>(type)]; }
    int32_t GetRandomParUPValue(BlockType type) const { return blockRandomParms_.randomParUPValue[static_cast<int32_t>(type)]; }
    int32_t GetLineCounter(BlockType type) const;
    int32_t GetRandomParRightOfAdvance(BlockType type) const { return blockRandomParms_.randomParRightofAdvance[static_cast<int32_t>(type)]; }
    float GetScoreValue(BlockType type) const { return scoreValue_[static_cast<int32_t>(type)]; }
    int32_t GetMoveTenpoNum() const { return moveTenpoNum_; }
    Easing GetScalingEasing() const { return scalingEase_; }
    Vec3f GetResultScale() const { return resultScale_; }
    bool GetIsMove() const { return isMove_; }
    float GetMoveTime() const { return moveTimemax_; }
    int32_t GetCurrentLevel() const { return currentLevel_; }

    /// setter
    void SetCurrentCostIncrement(BlockType type) { currentCosts_[static_cast<int32_t>(type)]++; }
    void SetIncrementLineCounter(BlockType type) { lineCounter_[static_cast<int32_t>(type)]++; }
    void SetEaseTime(const float& time) { scalingEase_.time = time; }
    void SetResultScale(const Vec3f resullt) { resultScale_ = resullt; }
    void SetEaseType(const EaseType& is) { easeType_ = is; }
    void SetIsMove(const bool& is) { isMove_ = is; }
};
