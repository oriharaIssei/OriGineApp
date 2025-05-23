#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"

#include <Entity.h>
#include <Vector2.h>

class PlayerAnimationStatus
    : public IComponent {
    friend void to_json(nlohmann::json& j, const PlayerAnimationStatus& m);
    friend void from_json(const nlohmann::json& j, PlayerAnimationStatus& m);

public:
    enum class PlayerAnimeStep {
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

    ///*launch
    float launchRotationValue_ = 0.0f;
    float launchRotate_ = 0.0f;
    // scale

    // ease
    Easing moveEasing_;
    Easing landingEasing_;
    Easing waitEasing_;
    Easing launchEasing_;
    Easing launchRotateEasing_;

public:
    PlayerAnimationStatus() {}
    virtual ~PlayerAnimationStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

    void MoveAnimaiton(const float& deltaTime);
    void WaitAnimation(const float& deltaTime);
    void LndingAnimaiton(const float& deltaTime);
    void LaunchScaleAnimation(const float& deltaTime);
    void LaunchRotateAnimation(const float& deltaTime);

    void Reset();

public: // accsessor
    /// getter
    bool GetIsAlive() const { return isAlive_; }
    const float& GetStartPosY() const { return startPosY_; }
    const float& GetEndPosY() const { return endPosY_; }
    const Vec3f& GetMaxScale() const { return landingScale_; }
};
