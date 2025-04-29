#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <array>
#include <cstdint>
#include <Entity.h>
#include <Vector3.h>

enum class LevelUIDigit {
    ICON,
    ONE,
    TWO,
    COUNT,
};

enum class AnimationStep {
    NONE,
    MOVE, // スケールも同時にアップ
    SCROLL,
    SCALING,
    REVERSE, // スケールも同時にダウン
};

class LevelUIParentStatus
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    int32_t currentLevel_ = 0; // 現在のレベル
    bool isLevelChange_   = false;

    // pos
    Vec3f basePos_ = {0, 0, 0};
    Vec3f initPos_ = {0, 0, 0};
    Vec3f easePos_ = {0, 0, 0};

    //scale
    Vec3f baseScale_         = {1, 1, 1};
    Vec3f easeScale_         = {1, 1, 1};
    Vec3f changingEaseScale_ = {1, 1, 1};
    Vec3f initScale_         = {1, 1, 1};

    // ease
    Easing moveEasing_;
    Easing scaleEasing_;
    Easing uvScrollEasing_;

    // uv
    float currentUVPos_ = 0.0f;
    float saveUVPos_    = 0.0f;

    // step
    AnimationStep curerntStep_ = AnimationStep::NONE;

public:
    LevelUIParentStatus() {}
    virtual ~LevelUIParentStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

    void MoveAnimation(const float& time);
    void ScrollAnimation(const float& time);
    void ScalingAnimation(const float& time);
    void ReverseAnimation(const float& time);

    //
    void Reset();

public: // accsessor
    /// getter
    int32_t GetCurrentLevel() const { return currentLevel_; } // 現在のコンボ数
    bool GetIsLevelChange() const { return isLevelChange_; }
    Vec3f GetBasePos() const { return basePos_; }
    Vec3f GetInitPos() const { return initPos_; }
    Vec3f GetEasePos() const { return easePos_; }
    Vec3f GetBaseScale() const { return baseScale_; }
    AnimationStep GetAnimationStep() const { return curerntStep_; }
    /// setter
    void LlvelIncrement() { currentLevel_++; } // 現在のコンボ数
    void SetIsLevelChange(const bool& currentComboNum) { isLevelChange_ = currentComboNum; }
    void SetbasePos(const Vec3f& pos) { basePos_ = pos; }
    void SetInitPos(const Vec3f& pos) { initPos_ = pos; }
    void SetAnimationStep(const AnimationStep& step) { curerntStep_ = step; }
};
