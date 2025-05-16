#pragma once

#include "component/Block/BlockManager.h"
#include "component/IComponent.h"
#include <array>
#include <cstdint>
#include <Vector3.h>
#include <Vector4.h>

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
    int32_t row_;
    int32_t columuNum_=0;

    int32_t currentHP_;
    bool isDestroy_ = false;
    bool isbreak_   = false;
    bool isNotScrapSpawn_ = false;

    // color
    Vec4f changeColor_ = {1.0f, 0.0f, 0.0f, 1.0f};

    // score
    float baseScoreValue_ = 0.0f;
    float plusScoreRate_  = 1.0f;

    BlockType blockType_;

    Vec3f preMovePos_ = {0.0f, 0.0f, 0.0f};
    bool isMove_      = false;
    Easing moveEase_;

    //time
    float breakOffsetTime_ = 0.0f;
    bool isBreakForAdvaltageEffect_=false;

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

public: // accsessor
    /// getter
    int32_t GetRow() const { return row_; }
    int32_t GetColum() const { return columuNum_; }
    int32_t GetCurrentHP() const { return currentHP_; }
    bool GetIsDestroy() const { return isDestroy_; }
    const bool& GetIsFall() const { return isFall_; }
    BlockType GetBlockType() const { return blockType_; }
    bool GetIsBreak() const { return isbreak_; }
    bool GetIsNotScrapSpawn() const { return isNotScrapSpawn_; }
    bool GetIsBreakForAdvantageEffect() const { return isBreakForAdvaltageEffect_; }
     float GetBaseScoreValue() const { return baseScoreValue_; }
    float GetPlusScoreRate() const { return plusScoreRate_; }
    Vec3f GetPreMovePos() const { preMovePos_; }
    Vec4f GetChangeColor() const { return changeColor_; }

    /// setter
    void setRow(const int32_t& colum);
    void SetColum(const int32_t& row) { columuNum_ = row; }
    void SetIsFall(const bool& is) { isFall_ = is; }
    void SetcurrentHP(const int32_t& hp) { currentHP_ = hp; }
    void SetIsDestroy(const bool& is) { isDestroy_ = is; }
    void SetBlockType(const BlockType& type) { blockType_ = type; }
    void SetIsBreak(const bool& is) { isbreak_ = is; }
    void SetBaseScoreValue(const float& value) { baseScoreValue_ = value; }
    void SetPreMovePos(const Vec3f& offset) { preMovePos_ = offset; }
    void SetIsMove(const bool& is) { isMove_ = is; }
    void SetEaseTimeMax(const float& time) { moveEase_.maxTime = time; }
    void SetPlusScoreRate(const float& ratio) {  plusScoreRate_ = ratio; }
    void SetIsBreakForAdvntageEffect(const bool& is) { isBreakForAdvaltageEffect_ = is; }
};
