#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <Entity.h>
#include <Vector2.h>
#include <Vector4.h>

enum class TimerAnimationStep {
    NONE,
    SCALING,
    END,
};

class TimerAnimationStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const TimerAnimationStatus& _component);
    friend void from_json(const nlohmann::json& _json, TimerAnimationStatus& _component);

private: // variables
    bool isAlive_ = true;

    bool isAnimation_ = false;

    // pos
    Vec2f basePos_ = {0.0f, 0.0f};

    // offset

    // scale
    Vec2f baseScale_ = {1.0f, 1.0f};
    Vec2f initScale_ = {1.0f, 1.0f};
    Vec2f maxScale_  = {1.0f, 1.0f};

    // back
    Vec2f backBaseScale_ = {1.0f, 1.0f};
    Vec2f backInitScale_ = {1.0f, 1.0f};
    Vec2f backMaxScale_  = {1.0f, 1.0f};

    Vec2f textureSize_;

    Vec4f panicColor_;
    Vec4f color_ = {1.0f, 1.0f, 1.0f, 1.0f};

    // ease
    Easing scalingEasing_;
    Easing colorChangeEasing_;

    // back
    Easing backApearEase_;
    Easing backAlphaEase_;
    float backAlpha_ = 0.0f;
    float backStartAlpha_;
    float backEndAlpha_;

    // step
    TimerAnimationStep curerntStep_ = TimerAnimationStep::NONE;

    // waitTime
    float waitTimeAfterScaing_;

public:
    TimerAnimationStatus() {}
    virtual ~TimerAnimationStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    void ScalingAnimation(const float& time);
    void ColorChangeEasing(const float& time);
    void CheckAbleNextStep();

    void BackApearScaling(const float& time);
    void BackAlphaEasing(const float& time);
    //
    void Reset();

public: // accsessor
    /// getter

    bool GetIsAnimation() const { return isAnimation_; }
    Vec2f GetBasePos() const { return basePos_; }
    Vec2f GetBaseScale() const { return baseScale_; }
    Vec4f GetBaseClolor() const { return color_; }
    Vec2f GetTextureSize() const { return textureSize_; }
    TimerAnimationStep GetAnimationStep() const { return curerntStep_; }
    float GetWaitTimeAfterApear() const { return waitTimeAfterScaing_; }
    float GetBackAlpha() const { return backAlpha_; }
    Vec2f GetBackBaseScale() const { return backBaseScale_; }

    /// setter
    void SetAnimationStep(const TimerAnimationStep& step) { curerntStep_ = step; }
    void SetIsResultAnimation(const bool& is) { isAnimation_ = is; }
};
