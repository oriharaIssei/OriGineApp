#pragma once

#include "component/IComponent.h"
#include "component/Piller/FloatingFloorAnimationStatus.h"
#include <array>
#include <cstdint>
#include <Vector3.h>

class FloatingFloorStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const FloatingFloorStatus& _block);
    friend void from_json(const nlohmann::json& _json, FloatingFloorStatus& _block);

private: // variables
    bool isAlive_ = true;
    bool isFall_  = false;

    int32_t columNum_;
    int32_t rowNum_;

    float startPosY_;
    float fallValue_;
    float fallPosY_;
    float fallspeed_;
    float fallEaseT_;
    int32_t currentHP_;
    int32_t HPMax_;

    bool isDestroy_    = false;
    bool isRevivaling_ = false;
    float revivalTime_;
    float currentRevivalTimer_ = 0.0f;
    Vec3f saveScale_           = {1.0f, 1.0f, 1.0f};

    float scoreUpRate_;
    float startScoreUpRate_;

    bool isPlayerUnderTheFloor_ = false;

    /// collider
    Vec3f fallCollisionSizeMin_;
    Vec3f fallCollisionSizeMax_;
    Vec3f fallCollisionCenterPos_;

    // pos
    Vec3f savePos_;
    Vec3f damageShakePos_={0.0f,0.0f,0.0f};

    // reaction
    ReactionStep reactionStep_=ReactionStep::NONE;
    float shakeTime_=0.0f;
    bool isDamageShake_       = false;

    public : FloatingFloorStatus() {}
    virtual ~FloatingFloorStatus() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Finalize() override;

    void TakeDamage();
    void TakeBigDamage();
    void RevivalReset();
    void DamageShakeReset(FloatingFloorAnimationStatus* animestatus);

    void SetColumDecrement();

    void ChangeDamageShake();

    void DamageShake(FloatingFloorAnimationStatus* animestatus,const float&deltaTime);

public: // accsessor
    /// getter
    bool GetIsDamageShake() const { return isDamageShake_; }
    int32_t GetColumNum() const { return columNum_; }
    int32_t GetRowNum() const { return rowNum_; }
    int32_t GetCurrentHP() const { return currentHP_; }
    float GetStartPosY() const { return startPosY_; }
    float GetFallValue() const { return fallValue_; }
    float GetFallPosY() const { return fallPosY_; }
    float GetFallEaseT() const { return fallEaseT_; }
    float GetFallSpeed() const { return fallspeed_; }
    float GetScoreUPRatio() const { return scoreUpRate_; }
    bool GetIsDestroy() const { return isDestroy_; }
    bool GetIsRevaviling() const { return isRevivaling_; }
    const bool& GetIsPlayerUnderTheFloor() const { return isPlayerUnderTheFloor_; }
    const bool& GetIsFall() const { return isFall_; }
    float GetRevivalTime() const { return revivalTime_; }
    float GetCurrentRevivalTime() const { return currentRevivalTimer_; }
    Vec3f GetSaveScale() const { return saveScale_; }
    Vec3f GetFallCollisionSizeMin() const { return fallCollisionSizeMin_; }
    Vec3f GetFallCollisionSizeMax() const { return fallCollisionSizeMax_; }
    Vec3f GetFallCollisionCenterPos() const { return fallCollisionCenterPos_; }
    Vec3f GetSavePos() const { return savePos_; }
    Vec3f GetDamageShakePos() const { return damageShakePos_; }
    ReactionStep GetReactionStep() const { return reactionStep_; }

    /// setter
    void SetColumAndRow(const int32_t& colum, const int32_t& row);
    void SetIsFall(const bool& is) { isFall_ = is; }
    void SetIsRevivaling(const bool& is) { isRevivaling_ = is; }
    void SetStartPosY(const float& pos) { startPosY_ = pos; }
    void SetFallValue(const float& value) { fallValue_ = value; }
    void SetFallPosY(const float& pos) { fallPosY_ = pos; }
    void SetFallEaseT(const float& t) { fallEaseT_ = t; }
    void SetIncrementFallEaseT(const float& t) { fallEaseT_ += t; }
    void SetFallSpeed(const float& speed) { fallspeed_ = speed; }
    void SetcurrentHP(const int32_t& hp) { currentHP_ = hp; }
    void SetIsDestroy(const bool& is) { isDestroy_ = is; }
    void SetRevivalTime(const float& speed) { revivalTime_ = speed; }
    void SetIncrementRevivalTime(const float& speed) { currentRevivalTimer_ += speed; }
    void SetHPMax(const int32_t max) { HPMax_ = max; }
    void SetSaveScale(const Vec3f& s) { saveScale_ = s; }
    void SetScoreUpRatio(const float& ratio) { scoreUpRate_ = ratio; }
    void SetStartScoreUPRate(const float& score) { startScoreUpRate_ = score; }
    void SetIsPlayerUnderTheFloor(const bool& is) { isPlayerUnderTheFloor_ = is; }
    void SetFallCollisionSizeMin(const Vec3f& s) { fallCollisionSizeMin_ = s; }
    void SetFallCollisionSizeMax(const Vec3f& s) { fallCollisionSizeMax_ = s; }
    void SetFallCollisionCenterPos(const Vec3f& s) { fallCollisionCenterPos_ = s; }
    void SetSavePos(const Vec3f pos) { savePos_ = pos; }
    void SetReactionStep(const ReactionStep& step) { reactionStep_ = step; }
};
