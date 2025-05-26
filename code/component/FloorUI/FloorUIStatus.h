#pragma once

#include "component/IComponent.h"
#include <Entity.h>
#include <Vector2.h>
#include <Vector3.h>
#include"KetaEasing.h"

enum class FloorUIStep {
    NONE,
    APPER,
    MOVING,
    CLOSE,
    END,
};


class FloorUIStatus
    : public IComponent {
    friend void to_json(nlohmann::json& j, const FloorUIStatus& m);
    friend void from_json(const nlohmann::json& j, FloorUIStatus& m);

private: // variables
    bool isAlive_ = true;

    const bool* isAnimation_;

    // pos
    Vec3f position_ = {0.0f, 0.0f, 0.0f};
    Vec3f startPos_ = {0.0f, 0.0f, 0.0f};
    Vec3f endPos_   = {0.0f, 0.0f, 0.0f};
    Vec3f basePosition_ = {0.0f, 0.0f, 0.0f};

    // scale
    Vec3f baseScale_ = {1.0f, 1.0f, 1.0f};
    Vec3f saveScale_ = {1.0f, 1.0f, 1.0f};
    Vec3f maxScale_  = {1.0f, 1.0f, 1.0f};

    FloorUIStep floorUIStep_ = FloorUIStep::NONE;

    Easing moveEasing_;
    Easing openEasing_;
    Easing closeEasing_;

    float rotate_      = 0.0f;
    float rotateSpeed_ = 0.0f;

public:
    FloorUIStatus() {}
    virtual ~FloorUIStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    void moveEasing(const float& time);
    void OpenEasing(const float& time);
    void CloseEasing(const float& time);
    void SavingScale();
    void RotateMoving(const float& deltatime);
    void ChangeAnimationEndForFlag();
    //
    void Reset();

public: // accsessor
        /// getter
public: // accessors
    /// getter
    float GetRotate() { return rotate_; }
    bool GetIsAlive() const { return isAlive_; }
    const bool* GetIsAnimationPtr() const { return isAnimation_; }
    const Vec3f& GetPosition() const { return position_; }
    const Vec3f& GetStartPos() const { return startPos_; }
    const Vec3f& GetEndPos() const { return endPos_; }
    const Vec3f& GetBaseScale() const { return baseScale_; }
    const Vec3f& GetStartScale() const { return maxScale_; }
    const Vec3f& GetBasePosition() const { return basePosition_; }  
    FloorUIStep GetFloorUIStep() const { return floorUIStep_; }

    const Easing& GetMoveEasing() const { return moveEasing_; }
    const Easing& GetOpenEasing() const { return openEasing_; }
    const Easing& GetCloseEasing() const { return closeEasing_; }

    /// setter
    void SetIsAlive(bool alive) { isAlive_ = alive; }
    void SetIsAnimation(const bool* anim) { isAnimation_ = anim; }

    void SetPosition(const Vec3f& pos) { position_ = pos; }
    void SetStartPos(const Vec3f& pos) { startPos_ = pos; }
    void SetEndPos(const Vec3f& pos) { endPos_ = pos; }

    void SetBaseScale(const Vec3f& scale) { baseScale_ = scale; }
    void SetMaxScale(const Vec3f& scale) { maxScale_ = scale; }

    void SetFloorUIStep(FloorUIStep step) { floorUIStep_ = step; }

    void SetMoveEasing(const Easing& easing) { moveEasing_ = easing; }
    void SetOpenEasing(const Easing& easing) { openEasing_ = easing; }
    void SetCloseEasing(const Easing& easing) { closeEasing_ = easing; }

    void SetFloorRotateSpeed(const float& speed) { rotateSpeed_ = speed; }

    void SetbasePosition(const Vec3f& pos) { basePosition_ = pos; }
};
