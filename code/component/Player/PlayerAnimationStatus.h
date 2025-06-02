#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"

#include <Entity.h>
#include <Vector2.h>

class Audio;
    //class BlockManager;
class PlayerAnimationStatus
    : public IComponent {
    friend void to_json(nlohmann::json& j, const PlayerAnimationStatus& m);
    friend void from_json(const nlohmann::json& j, PlayerAnimationStatus& m);

public:
    enum class MotionStep {
        WAIT,
        MOVE,
        LAUNCH,
    };

private: // variables
    bool isAlive_ = true;

    Vec3f baseScale_ = {1.0f, 1.0f, 1.0f};
    Vec3f animationScale_ = {1.0f, 1.0f, 1.0f};
    float baseYOffset_;
    ///* Moving
    // pos
    float jumpPosY_ = 0.0f;
    float startPosY_ = 0.0f;
    float endPosY_   = 0.0f;
    // scale
    Vec3f landingScale_ = {1.0f, 1.0f, 1.0f};

    ///* Waiting
    Vec3f waitScale_ = {1.0f, 1.0f, 1.0f};
    Vec3f waitScaleOffset_ = {1.0f, 1.0f, 1.0f};

    ///*launch
    float launchRotationValue_ = 0.0f;
    float launchRotate_ = 0.0f;
    Vec3f launchScale_   = {0.0f, 0.0f, 0.0f};
    // scale

    // ease
    Easing moveEasing_;
    Easing landingEasing_;
    Easing waitEasing_;
    Easing launchEasing_;
    Easing launchRotateEasing_;

    MotionStep motionStep_;

    bool isLanding_;

    bool isMoveSound_=false;

public:
    PlayerAnimationStatus() {}
    virtual ~PlayerAnimationStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

    void MoveAnimaiton(const float& deltaTime, Audio* audio);
    void WaitAnimation(const float& deltaTime, const Vec3f& blockSize);
    void LandingAnimation(const float& deltaTime);
    void LaunchScaleAnimation(const float& deltaTime);
    void LaunchRotateAnimation(const float& deltaTime);

    void ChangeMotion(const MotionStep &step);
    void ChangeMotionWait();
    void Reset();

    void MoveSound(Audio* audio);

public: // accsessor
    /// getter
    bool GetIsAlive() const { return isAlive_; }
    MotionStep GetPlayerMotionStep() const { return motionStep_; }
    const float& GetJumpPosY() const { return jumpPosY_; }
    const float& GetBaseYOffset() const { return baseYOffset_; }
    const Vec3f& GetAnimationScale() const { return animationScale_; }
    const Vec3f& GetBaseScale() const { return baseScale_; }
    const Vec3f& GetLaunchScale() const { return launchScale_; }
    const float& GetLaunchRotateY() const { return launchRotate_; }

};
