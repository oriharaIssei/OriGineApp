#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <array>
#include <cstdint>
#include <Entity.h>
#include <Vector3.h>
#include <Vector2.h>

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

  
    bool isLevelChange_   = false;

    // pos
    Vec3f basePos_ = {0.0f, 0.0f, 0.0f};
    Vec3f initPos_ = {0.0f, 0.0f, 0.0f};
    Vec3f easePos_ = {0.0f, 0.0f, 0.0f};

    //scale
    Vec2f baseScale_         = {1.0f, 1.0f};
    Vec2f easeScale_         = {1.0f, 1.0f};
    Vec2f changingEaseScale_ = {1.0f, 1.0f};
    Vec2f initScale_         = {1.0f, 1.0f};

    // ease
    Easing moveEasing_;
    Easing scaleEasing_;
    Easing uvScrollEasing_;

    // uv
    float currentLevelUV_ = 0.0f; // 現在のレベル
    float nextLevelUV_ = 0.0f;
    float preLevelUV_     = 0.0f;

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
    void Init();

public: // accsessor
    /// getter
    float GetCurrentLevelUV() const { return currentLevelUV_; } // 現在のコンボ数
    bool GetIsLevelChange() const { return isLevelChange_; }
    Vec3f GetBasePos() const { return basePos_; }
    Vec3f GetInitPos() const { return initPos_; }
    Vec2f GetBaseScale() const { return baseScale_; }
    AnimationStep GetAnimationStep() const { return curerntStep_; }
    /// setter
    void LlvelIncrement() { currentLevelUV_++; } // 現在のコンボ数
    void SetIsLevelChange(const bool& currentComboNum) { isLevelChange_ = currentComboNum; }
   /* void SetbasePos(const Vec3f& pos) { basePos_ = pos; }
    void SetInitPos(const Vec3f& pos) { initPos_ = pos; }*/
    void SetAnimationStep(const AnimationStep& step) { curerntStep_ = step; }
    void SetNextLevelUV(const float& nextlevel) { nextLevelUV_ = nextlevel; }
};
