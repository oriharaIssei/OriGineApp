#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <array>
#include <cstdint>
#include <Entity.h>
#include <Vector2.h>
#include <Vector3.h>

enum class KeyResultStep {
    NONE,
    ALPHA,
    END,
};

class ResultUIkeyStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const ResultUIkeyStatus& _component);
    friend void from_json(const nlohmann::json& _json, ResultUIkeyStatus& _component);

private: // variables
    bool isAlive_ = true;

    bool isAnimation_ = false;

    // pos
    Vec3f basePos_ = {0.0f, 0.0f, 0.0f};
    Vec3f initPos_ = {0.0f, 0.0f, 0.0f};
    Vec3f easePos_ = {0.0f, 0.0f, 0.0f};

    // offset

    // ease
    Easing moveEasing_;
    Easing alphaEasing_;

    // uv
    float alpha_ = 0.0f; // 現在のレベル

    // step
    KeyResultStep curerntStep_ = KeyResultStep::NONE;

public:
    ResultUIkeyStatus() {}
    virtual ~ResultUIkeyStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    void MoveAnimation(const float& time);
    void AlphaAnimation(const float& time);

    //
    void Reset();

public: // accsessor
    /// getter

    bool GetIsAnimation() const { return isAnimation_; }
    Vec3f GetBasePos() const { return basePos_; }
    Vec3f GetInitPos() const { return initPos_; }
    KeyResultStep GetAnimationStep() const { return curerntStep_; }
    float GetAlpha() const { return alpha_; }

    /// setter

    void SetIsAnimation(const bool& currentComboNum) { isAnimation_ = currentComboNum; }
    void SetInitPos(const Vec3f& pos) { initPos_ = pos; }
    void SetAnimationStep(const KeyResultStep& step) { curerntStep_ = step; }
};
