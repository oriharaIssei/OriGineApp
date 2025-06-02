#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <Entity.h>
#include <Vector3.h>

class EffectByBlockAnimationStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const EffectByBlockAnimationStatus& _component);
    friend void from_json(const nlohmann::json& _json, EffectByBlockAnimationStatus& _component);

public:
    enum class AnimationStep {
        START,
        APEAR,
        WAIT,
        FADEOUT,
        END,
    };

private: // variables
    bool isAlive_ = true;

    Easing ApearEasing_;
    Easing FadeOutEasing_;
    Vec3f maxScale_;
    float rotateSpeed_;
    float fadeOutTime_;
    float fadeOutValueX_;

    //
    Vec3f baseScale_;
    float savePosX_;
    float basePosX_;
    float rotate_;
    float currentTime_;

    AnimationStep animationStep_ = AnimationStep::START;

public:
    EffectByBlockAnimationStatus() {}
    virtual ~EffectByBlockAnimationStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    void FadeOutEasing(const float& deltaTime);
    void ApearEasing(const float& deltaTime);
    void FadeOutWaitUpdate(const float& deltaTime, const float& savePos);
    void Reset();

public: // accsessor
        /// getter
    AnimationStep GetAnimationStep() const { return animationStep_; }
    const Vec3f& GetBaseScale() const { return baseScale_; }
    float GetSavePosX() const { return savePosX_; }
    float GetBasePosX() const { return basePosX_; }
    float GetRotate() const { return rotate_; }
    /// setter
    void SetApearEasing(const Easing& easing) { ApearEasing_ = easing; }
    void SetFadeOutEasing(const Easing& easing) { FadeOutEasing_ = easing; }
    void SetMaxScale(const Vec3f& scale) { maxScale_ = scale; }
    void SetRotateSpeed(float speed) { rotateSpeed_ = speed; }
    void SetFadeOutTime(float time) { fadeOutTime_ = time; }
    void SetFadeOutValueX(float value) { fadeOutValueX_ = value; }
    void SetAnimationStep(AnimationStep step) { animationStep_ = step; }
};
