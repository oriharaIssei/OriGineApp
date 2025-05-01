#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <array>
#include <cstdint>
#include <Entity.h>
#include <Vector2.h>
#include <Vector3.h>

enum class ResultFontStep {
    NONE,
    FIRSTWAIT,
    ZOOM,
    MOVEWAIT,
    MOVE,
    END,
};
class ResultUIParentStatus;
class ResultFontStatus
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    bool isresultAnimation = false;

    // pos
    Vec3f basePos_   = {0.0f, 0.0f, 0.0f};
    Vec3f initPos_   = {0.0f, 0.0f, 0.0f};
    Vec3f easePos_   = {0.0f, 0.0f, 0.0f};
    Vec3f savePos_   = {0.0f, 0.0f, 0.0f};
    float zoomValue_ =  0.0f;

    // offset

    // scale
    Vec2f baseScale_      = {0.0f, 0.0f};
    Vec2f easeScale_ = {1.0f, 1.0f};

    // ease
    Easing moveEasing_;
    Easing zoomEasing_;

    // step
    ResultFontStep curerntStep_ = ResultFontStep::NONE;

    // waitTime
    float moveWaitTime_;
    float firstWaitTime_;

public:
    ResultFontStatus() {}
    virtual ~ResultFontStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

    void MoveAnimation(const float& time, ResultUIParentStatus& isnext);
    void ZoomAnimation(const float& time);

    //
    void Reset();

public: // accsessor
    /// getter

    bool GetIsResultAnimation() const { return isresultAnimation; }
    Vec3f GetBasePos() const { return basePos_; }
    Vec3f GetInitPos() const { return initPos_; }
    Vec2f GetBaseScale() const { return baseScale_; }
    ResultFontStep GetAnimationStep() const { return curerntStep_; }
    float GetMoveWaitTime() const { return moveWaitTime_; }
    float GetFirstWaitTime() const { return firstWaitTime_; }

    /// setter
    void SetInitPos(const Vec3f& pos) { initPos_ = pos; }
    void SetAnimationStep(const ResultFontStep& step) { curerntStep_ = step; }
    void SetIsResultAnimation(const bool& is) { isresultAnimation = is; }
};
