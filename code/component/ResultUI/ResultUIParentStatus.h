#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <array>
#include <cstdint>
#include <Entity.h>
#include <Vector2.h>
#include <Vector3.h>

enum class ResultStep {
    NONE,
    ALPHA,
    SCOREUPWAIT,
    SCOREUP,
    SCOREUPSCALING,
    END,
};

class ResultUIParentStatus
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    bool isAnimation_ = false;
    bool isRankApeearAnimationStart_=false;

    // pos
    Vec3f basePos_ = {0.0f, 0.0f, 0.0f};
    Vec3f initPos_ = {0.0f, 0.0f, 0.0f};
    Vec3f easePos_ = {0.0f, 0.0f, 0.0f};

    // offset

    // scale
    Vec2f baseScale_      = {1.0f, 1.0f};
    Vec2f scoreScale_     = {1.0f, 1.0f};
    Vec2f scoreEaseScale_ = {1.0f, 1.0f};
    Vec2f scoreInitScale_ = {1.0f, 1.0f};

    // ease
    Easing moveEasing_;
    Easing scaleEasing_;
    Easing alphaEasing_;

    // uv
    float alpha_ = 0.0f; // 現在のレベル

    // score
    float currentScore_ = 0.0f;
    float resultScore_  = 0.0f;
    float scoreUpSpeed_;

    // step
    ResultStep curerntStep_ = ResultStep::NONE;

    // waitTime
    float scoreWaitTime_;

    //
    bool hasStartedScaling_;

public:
    ResultUIParentStatus() {}
    virtual ~ResultUIParentStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

    void MoveAnimation(const float& time);
    void AlphaAnimation(const float& time);
    void ScoreScalingAnimation(const float& time);
    void ScoreUP(const float& time);

    //
    void Reset();

public: // accsessor
    /// getter

    bool GetIsAnimation() const { return isAnimation_; }
    bool GetIsRankApeearAnimationStart() const { return isRankApeearAnimationStart_; }
    Vec3f GetBasePos() const { return basePos_; }
    Vec3f GetInitPos() const { return initPos_; }
    Vec2f GetScoreScale() const { return scoreScale_; }
    Vec2f GetBaseScale() const { return baseScale_; }
    ResultStep GetAnimationStep() const { return curerntStep_; }
    float GetScoreWaitTime() const { return scoreWaitTime_; }
    float GetCurrentScore() const { return currentScore_; }
    float GetAlpha() const { return alpha_; }

    /// setter

    void SetIsAnimation(const bool& currentComboNum) { isAnimation_ = currentComboNum; }
    void SetInitPos(const Vec3f& pos) { initPos_ = pos; }
    void SetAnimationStep(const ResultStep& step) { curerntStep_ = step; }
    void SetResultScore(const float& score) { resultScore_ = score; }
    void SetIsRankApeearAnimationStart(const bool& is) { isRankApeearAnimationStart_ = is; }
};
