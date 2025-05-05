#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <array>
#include <cstdint>
#include <Entity.h>
#include <Vector2.h>
#include <Vector3.h>

enum class MenuMode {
    NONE,
    MENUSELECT,
    WATCHINGTUTORIAL,
    CLOSEINIT,
    CLOSE,
    END,
    GAMEEND,
};

class TutorialMenuParentStatus;
class Input;
class MenuStatus
    : public IComponent {

    friend void to_json(nlohmann::json& j, const MenuStatus& m);
    friend void from_json(const nlohmann::json& j, MenuStatus& m);

public:
    enum class MenuCategory {
        RETURNGAME,
        OPENTUTORIAL,
        RETURNTITLE,

    };

private: // variables
    bool isAlive_ = true;

    bool isAnimation_;
   
    // pos
    Vec2f position_ = {0.0f, 0.0f};
    Vec2f arrowPos_ = {0.0f, 0.0f};
    std::array<Vec2f, 3> arrowPositions_;

    // offset

    // scale
    Vec2f baseScale_ = {0.0f, 1.0f};

    // ease
    Easing moveEasing_;
    Easing apperUVEasing_;

    /// uv
    float scaleX_ = 0.0f;

    /// pauge
    MenuCategory currentCategory_ = MenuCategory::RETURNGAME;
    int32_t maxCategoryNum_       = 0;

    // step
    MenuMode scrollStep_ = MenuMode::NONE;

public:
    MenuStatus() {}
    virtual ~MenuStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
   
    virtual void Finalize();

    void OpenMenuAnimation(const float& time);
    void CloseAnimation(const float& time);
    //
    void Reset();
    void ScrollTimeReset();

    void UpdateArrowPos();
    void SelectPreviousCategory();
    void SelectNextCategory();

   
    void DesideForCategory(Input* input, TutorialMenuParentStatus* tutorialStatus);

public: // accsessor
        /// getter
    bool GetIsAnimation() const { return isAnimation_; }
    bool GetIsPose();
    float GetScaleX() const { return scaleX_; }
    Vec2f GetBasePos() const { return position_; }
    Vec2f GetArrowPos() const { return arrowPos_; }
    Vec2f GetBaseScale() const { return baseScale_; }
    MenuMode GetMenuMode() const { return scrollStep_; }

    /// setter
    /* void CurrentPaugeIncrement() { currentPauge_++; }
     void CurrentPaugeDecrement() { currentPauge_--; }*/
    void SetMenuMode(const MenuMode& step) { scrollStep_ = step; }
    void SetIsAnimation(const bool& is) { isAnimation_ = is; }
};
