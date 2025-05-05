#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <array>
#include <cstdint>
#include <Entity.h>
#include <Vector2.h>
#include <Vector3.h>

enum class ScrollStep {
    NONE,
    FIRSTUVSCROLL,
    PAUGESELECTION,
    PAUGEUP,
    PAUGEDOWN,
    BACK,
    END,
};

class TutorialMenuParentStatus
    : public IComponent {
    friend void to_json(nlohmann::json& j, const TutorialMenuParentStatus& m);
    friend void from_json(const nlohmann::json& j, TutorialMenuParentStatus& m);

private: // variables
    bool isAlive_ = true;

    bool isAnimation_ = false;

    // pos
    Vec2f position_    = {0.0f, 0.0f};
    Vec2f initPos_     = {0.0f, 0.0f};
    Vec2f startPos_    = {0.0f, 0.0f};
    Vec2f endPos_      = {0.0f, 0.0f};
    Vec2f savePos_     = {0.0f, 0.0f};
    Vec2f scrollValue_ = {0.0f, 0.0f};

    // offset

    // scale
    Vec2f baseScale_ = {1.0f, 1.0f};

    // ease
    Easing moveEasing_;
    Easing apperUVEasing_;

    /// uv
    float scaleX_    = 0.0f;

    /// pauge
    int32_t currentPauge_ = 0;
    int32_t maxPauge_     = 0;

    // step
    ScrollStep scrollStep_ = ScrollStep::NONE;

public:
    TutorialMenuParentStatus() {}
    virtual ~TutorialMenuParentStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
  

    virtual void Finalize();

    void MoveAnimation(const float& time);
    void FirstMoveAnimation(const float& time);
    void BackSizeAnimation(const float& time);
    //
    void Reset();
    void ScrollTimeReset();

public: // accsessor
    /// getter

    bool GetIsAnimation() const { return isAnimation_; }
    float GetScaleX() const { return scaleX_; }
    Vec2f GetBasePos() const { return position_; }
    Vec2f GetBaseScale() const { return baseScale_; }
    ScrollStep GetAnimationStep() const { return scrollStep_; }

    /// setter
   /* void CurrentPaugeIncrement() { currentPauge_++; }
    void CurrentPaugeDecrement() { currentPauge_--; }*/
    void SetAnimationStep(const ScrollStep& step) { scrollStep_ = step; }
    void SetIsAnimation(const bool& is) { isAnimation_ = is; }
};
