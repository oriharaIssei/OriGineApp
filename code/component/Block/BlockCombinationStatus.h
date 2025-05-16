#pragma once

#include "component/Block/BlockManager.h"
#include "component/IComponent.h"
#include <cstdint>
#include <vector>
#include <Vector3.h>

class BlockStatus;
class BlockCombinationStatus
    : public IComponent {
public:
    friend void to_json(nlohmann::json& _json, const BlockCombinationStatus& _block);
    friend void from_json(const nlohmann::json& _json, BlockCombinationStatus& _block);

private: // variables
    bool isAlive_ = true;

    int32_t conbinationMax_;
    std::vector<BlockStatus*> blockStatusArray_;
    float plusTimerRate_;
    float plusTimerValue_;
    float plusScoreRate_;
    float plusScoreValue_;
    float breakOffsetTime_;

    float minusScoreValue_;
    float minusTimerValue_;

    Vec3f timerUIPosOffset_;
    Vec3f scoreUIPosOffset_;

public:
    BlockCombinationStatus() {}
    virtual ~BlockCombinationStatus() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Finalize() override;

    void AddBlockStatus(BlockStatus* status);
    std::vector<BlockStatus*> GetRightBlocks(const int& baseRowNum,const int&columNum);

public: // accsessor
    /// getter
    Vec3f GetTimerUIPosOffset() const { return timerUIPosOffset_; }
    Vec3f GetScoreUIPosOffset() const { return scoreUIPosOffset_; }
    float GetPlusRate() const { return plusTimerRate_; }
    float GetPlusValue() const { return plusTimerValue_; }
    float GetPlusScoreRate() const { return plusScoreRate_; }
    float GetMinusTimerValue() const { return minusTimerValue_; }
    float GetMinusScoreValue() const { return minusScoreValue_; }
    float GetPlusScoreValue() const { return plusScoreValue_; }
    float GetBreakOffsetTime() const { return breakOffsetTime_; }
    const std::vector<BlockStatus*>& GetBlockStatusArray() const {return blockStatusArray_;}

    /// setter
    void SetMinusTimerValue(const float& timer) { minusTimerValue_ = timer; }
    void SetMinusScoreValue(const float& timer) { minusScoreValue_ = timer; }
};
