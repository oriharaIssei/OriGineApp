#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <array>
#include <cstdint>
#include <Entity.h>
#include <Vector2.h>
#include <Vector3.h>

enum class ArrowDirection {
   LEFT,
   RIGHT,
   COUNT,
};

class TutorialArrowStatus
    : public IComponent {
    friend void to_json(nlohmann::json& j, const TutorialArrowStatus& m);
    friend void from_json(const nlohmann::json& j, TutorialArrowStatus& m);

private: // variables
    bool isAlive_ = true;

    bool isAnimation_ = false;

    // pos
    Vec2f position_    = {0.0f, 0.0f};
    Vec2f startPos_    = {0.0f, 0.0f};
    Vec2f endPos_      = {0.0f, 0.0f};
  
    // offset

    // scale
    Vec2f baseScale_ = {1.0f, 1.0f};

    // ease
    Easing moveEasing_;
  
    // step
    ArrowDirection arrowDirection_;

public:
    TutorialArrowStatus() {}
    virtual ~TutorialArrowStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
  

    virtual void Finalize();

    void ArrowMoveEasing(const float& time);
    //
    void Reset();
  
public: // accsessor
    /// getter

    bool GetIsAnimation() const { return isAnimation_; }
    Vec2f GetBasePos() const { return position_; }
    Vec2f GetBaseScale() const { return baseScale_; }
    ArrowDirection GetArrowDirection() const { return arrowDirection_; }

    /// setter
   /* void CurrentPaugeIncrement() { currentPauge_++; }
    void CurrentPaugeDecrement() { currentPauge_--; }*/
    void SetArrowDirection(const ArrowDirection& step) { arrowDirection_ = step; }
    void SetIsAnimation(const bool& is) { isAnimation_ = is; }
};
