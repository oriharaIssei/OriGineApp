#pragma once

#include "component/IComponent.h"
#include <array>
#include <cstdint>
#include <Vector3.h>
#include <Vector4.h>
#include "KetaEasing.h"
#include "component/Block/BlockManager.h"

struct Transform;
class BlockStatus
    : public IComponent {
public:
    friend void to_json(nlohmann::json& _json, const BlockStatus& _block);
    friend void from_json(const nlohmann::json& _json, BlockStatus& _block);

private: // variables
    bool isAlive_ = true;
    bool isFall_  = false;

    //
    int32_t columnNum_;
    int32_t rowNum_ = 0;

    int32_t currentHP_;
    bool isDestroy_       = false;
    bool isbreak_         = false;
    bool isNotScrapSpawn_ = false;
    bool isMove_          = false;
    Easing moveEase_;

    // pos,color
    Vec3f preMovePos_           = {0.0f, 0.0f, 0.0f};
    Vec4f normalChangeColor_    = {1.0f, 0.0f, 0.0f, 1.0f};
    Vec4f skullChangeColor_     = {0.57f, 0.439f, 0.859f, 1.0f};
    Vec4f advantageChangeColor_ = {1.0f, 0.843f, 0.0f, 1.0f};

    // score
    float baseScoreValue_ = 0.0f;
    float plusScoreRate_  = 1.0f;

    // type
    BlockType blockType_;

    // time
    float breakOffsetTime_          = 0.0f;
    bool isBreakForAdvaltageEffect_ = false;

    float resultScore_ = 0.0f;
    float resultTime_  = 0.0f;
    bool isRightEdge_  = 0.0f;
    bool isColorChange_   = false;

    Easing breakApearEasing_;
    float startZPos_;
    float EndZPos_;
    float zposition_;
    

public:
    BlockStatus() {}
    virtual ~BlockStatus() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Finalize() override;

    void TakeDamageForBomb();
    void TakeDamageForBigBomb();
    void TakeDamageForFloor();

    void MoveUpdate(const float& time, Transform* transform, const float& moveValue);
    void TimeInit();

    void BreakTimerDecrement(const float& deltaTime);

    void SetBlockManagerParm(BlockManager* parm);

public: // accsessor
    /// getter
    bool GetIsColorChange() const { return isColorChange_; }
    bool GetIsRightEdge() const { return isRightEdge_; }
    float GetResultScore() const { return resultScore_; }
    float GetResultTime() const { return resultTime_; }
    float GetBreakOffsetTime() const { return breakOffsetTime_; }
    int32_t GetColumnNum() const { return columnNum_; }
    int32_t GetRowNum() const { return rowNum_; }
    int32_t GetCurrentHP() const { return currentHP_; }
    bool GetIsDestroy() const { return isDestroy_; }
    const bool& GetIsFall() const { return isFall_; }
    BlockType GetBlockType() const { return blockType_; }
    bool GetIsBreak() const { return isbreak_; }
    bool GetIsNotScrapSpawn() const { return isNotScrapSpawn_; }
    bool GetIsBreakForAdvantageEffect() const { return isBreakForAdvaltageEffect_; }
    float GetBaseScoreValue() const { return baseScoreValue_; }
    float GetPlusScoreRate() const { return plusScoreRate_; }
    Vec3f GetPreMovePos() const { return preMovePos_; }
    Vec4f GetNormalChangeColor() const { return normalChangeColor_; }
    Vec4f GetSkullChangeColor() const { return skullChangeColor_; }
    Vec4f GetAdvantageChangeColor() const { return advantageChangeColor_; }

    /// setter
    void SetIsColorChange(const bool& is) { isColorChange_ = is; }
    void SetBreakOffsetTime(const float& time) { breakOffsetTime_ = time; }
    void SetColumnNum(const int32_t& colum) {columnNum_ = colum;}
    void SetRowNum(const int32_t& row) { rowNum_ = row; }
    void SetIsFall(const bool& is) { isFall_ = is; }
    void SetcurrentHP(const int32_t& hp) { currentHP_ = hp; }
    void SetIsDestroy(const bool& is) { isDestroy_ = is; }
    void SetBlockType(const BlockType& type) { blockType_ = type; }
    void SetIsBreak(const bool& is) { isbreak_ = is; }
    void SetBaseScoreValue(const float& value) { baseScoreValue_ = value; }
    void SetPreMovePos(const Vec3f& offset) { preMovePos_ = offset; }
    void SetIsMove(const bool& is) { isMove_ = is; }
    void SetEaseTimeMax(const float& time) { moveEase_.maxTime = time; }
    void SetPlusScoreRate(const float& ratio) { plusScoreRate_ = ratio; }
    void SetIsBreakForAdvntageEffect(const bool& is) { isBreakForAdvaltageEffect_ = is; }
    void SetResultScore(const float& score) { resultScore_ = score; }
    void SetResultTimer(const float& timer) { resultTime_ = timer; }
    void SetIsRightEdge(const bool& is) { isRightEdge_ = is; }
};
