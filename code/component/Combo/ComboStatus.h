#pragma once

#include "component/IComponent.h"
#include <array>
#include <component/transform/Transform.h>
#include <cstdint>
#include <Entity.h>
#include <Vector3.h>

class ComboStatus
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    int32_t currentComboNum_ = 0; // 現在のコンボ数
    int32_t maxCombvoNum_    = 0; // 最大コンボ数
    bool isUpdateCombo_      = false;
    float continuationTime_;
    float currentTime_       = 0.0f;
   

public:
    ComboStatus() {}
    virtual ~ComboStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

public: // accsessor
    /// getter
    int32_t GetCurrentComboNum() const { return currentComboNum_; } // 現在のコンボ数
    int32_t GetMaxComboNum() const { return maxCombvoNum_; } // 最大コンボ数
    bool GetIsUpdateCombo() const { return isUpdateCombo_; } // コンボ更新フラグ
    float GetContinuationTime() const { return continuationTime_; } // コンボ継続時間
    float GetCurrentTimer() const { return currentTime_; } // 現在の時間
    /// setter
    void SetComboIncrement() { currentComboNum_++; } // 現在のコンボ数
    void SetCurrentCombo(const int32_t& currentComboNum) { currentComboNum_ = currentComboNum; } // 現在のコンボ数
    void SetMaxComboNum(const int32_t& maxComboNum) { maxCombvoNum_ = maxComboNum; } // 最大コンボ数
    void SetIsUpdateCombo(const bool& isUpdateCombo) { isUpdateCombo_ = isUpdateCombo; } // コンボ更新フラグ
    void SetContinuationTime(const float& continuationTime) { continuationTime_ = continuationTime; } // コンボ継続時間
    void SetCurrentTime(const float& currentTime) { currentTime_ = currentTime; } // 現在の時間
    void SetCurerntTimeDecrement(const float& currentTime) { currentTime_ -= currentTime; } // 現在の時間
};
