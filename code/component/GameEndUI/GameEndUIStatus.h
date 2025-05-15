#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <Entity.h>
#include <Vector2.h>

enum class GameEndUIStep {
    NONE,
    APEER,//拡大＆アルファ
    WAIT, //待機
    END,

};

class GameEndUIStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const GameEndUIStatus& _component);
    friend void from_json(const nlohmann::json& _json, GameEndUIStatus& _component);

private: // variables
    bool isAlive_ = true;

    bool isAnimation_ = false;

    // pos
    Vec2f basePos_   = {0.0f, 0.0f};

    // offset

    // scale
    Vec2f baseScale_ = {0.0f, 0.0f};
    Vec2f maxScale_ = {1.0f, 1.0f};

    //alpha
    float alpha_ = 0.0f;

    // ease
  /*  Easing moveEasing_;*/
    Easing apeerEasing_;
    Easing alphaEasing_;

    // step
    GameEndUIStep curerntStep_ = GameEndUIStep::NONE;

    // waitTime
    float waitTimeAfterApearEase_;

public:
    GameEndUIStatus() {}
    virtual ~GameEndUIStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    void ApeerUIAnimation(const float& time);
    void AlphaEaseAnimation(const float& time);
    void CheckAbleNextStep();

    //
    void Reset();

public: // accsessor
    /// getter

    bool GetIsAnimation() const { return isAnimation_; }
    Vec2f GetBasePos() const { return basePos_; }
    Vec2f GetBaseScale() const { return baseScale_; }
    GameEndUIStep GetAnimationStep() const { return curerntStep_; }
    float GetWaitTimeAfterApear() const { return waitTimeAfterApearEase_; }

    /// setter
    void SetAnimationStep(const GameEndUIStep& step) { curerntStep_ = step; }
    void SetIsResultAnimation(const bool& is) { isAnimation_ = is; }
};
