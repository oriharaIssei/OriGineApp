#pragma once

#include "component/IComponent.h"
#include<array>
#include <cstdint>
#include<Vector3.h>

class FloatingFloorStatus
    : public IComponent {

private: // variables

    bool isAlive_ = true;
    bool isFall_ = false;
   

    int32_t columNum_;
    int32_t rowNum_;

    float startPosY_;
    float fallValue_;
    float fallPosY_;
    float fallspeed_;
    float fallEaseT_;
    int32_t currentHP_;
    int32_t HPMax_;

    bool isDestroy_ = false;
    bool isRevivaling_ = false;
    float revivalTime_;
    float currentRevivalTimer_ = 0.0f;
    Vec3f saveScale_           = {1.0f,1.0f,1.0f};

    float ratio_;

public:
    FloatingFloorStatus() {}
    virtual ~FloatingFloorStatus() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Save(BinaryWriter& _writer) override;
    void Load(BinaryReader& _reader) override;
    void Finalize() override;

    void TakeDamage();
    void RevivalReset();

  
 void SetColumDecrement();
   

public: // accsessor

    /// getter
    int32_t GetColumNum() const { return columNum_; }
    int32_t GetRowNum() const { return rowNum_; }
    int32_t GetCurrentHP() const { return currentHP_; }
    float GetStartPosY() const { return startPosY_; }
    float GetFallValue() const { return fallValue_; }
    float GetFallPosY() const { return fallPosY_; }
    float GetFallEaseT() const { return fallEaseT_; }
    float GetFallSpeed() const { return fallspeed_; }
    float GetRatio() const { return ratio_; }
    bool GetIsDestroy() const { return isDestroy_; }
    bool GetIsRevaviling() const { return isRevivaling_; }
    const bool& GetIsFall() const { return isFall_; }
    float GetRevivalTime() const { return revivalTime_; }
    float GetCurrentRevivalTime() const { return currentRevivalTimer_; }
    Vec3f GetSaveScale() const { return saveScale_; }

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
    void SetIsDestroy(const bool& is) { isDestroy_ = is;}
    void SetRevivalTime(const float& speed) { revivalTime_ = speed; }
    void SetIncrementRevivalTime(const float& speed) { currentRevivalTimer_ += speed; }
    void SetHPMax(const int32_t max) { HPMax_ = max; }
    void SetSaveScale(const Vec3f& s) { saveScale_ = s; }
    void SetRatio(const float& ratio) { ratio_ = ratio; }
};
