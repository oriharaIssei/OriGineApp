#pragma once
/// parent
#include "component/IComponent.h"

/// stl
#include <vector>

/// engine
#include <input/Input.h>
// component
#include "component/renderer/Sprite.h"

/// math
#include <Vector4.h>
#include <stdint.h>

///=====================================================
// Sprite を Button として扱うためのクラス
///=====================================================
class Button
    : public IComponent {
    friend void to_json(nlohmann::json& j, const Button& r);
    friend void from_json(const nlohmann::json& j, Button& r);

public:
    Button();
    ~Button();

    void Initialize(GameEntity* _entity);

    void Edit(Scene* _scene, GameEntity* _entity, const std::string& _parentLabel);

    void Finalize();

private:
    bool isHovered_  = false;
    bool isPressed_  = false;
    bool isReleased_ = false;

    std::vector<PadButton> shortcutPadButton_;
    std::vector<Key> shortcutKey_;

    Vec4f normalColor_  = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    Vec4f hoverColor_   = Vec4f(0.8f, 0.8f, 0.8f, 1.0f);
    Vec4f pressColor_   = Vec4f(0.6f, 0.6f, 0.6f, 1.0f);
    Vec4f releaseColor_ = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);

public: // getter setter
    const std::vector<PadButton>& getShortcutPadButton() const {
        return shortcutPadButton_;
    }
    const std::vector<Key>& getShortcutKey() const {
        return shortcutKey_;
    }

    bool isHovered() const {
        return isHovered_;
    }
    void setHovered(bool _hovered) {
        isHovered_ = _hovered;
    }

    bool isPressed() const {
        return isPressed_;
    }
    void setPressed(bool _pressed) {
        isPressed_ = _pressed;
    }

    bool isReleased() const {
        return isReleased_;
    }
    void setReleased(bool _released) {
        isReleased_ = _released;
    }

    const Vec4f& getNormalColor() const {
        return normalColor_;
    }
    void setNormalColor(const Vec4f& _color) {
        normalColor_ = _color;
    }

    const Vec4f& getHoverColor() const {
        return hoverColor_;
    }
    void setHoverColor(const Vec4f& _color) {
        hoverColor_ = _color;
    }

    const Vec4f& getPressColor() const {
        return pressColor_;
    }
    void setPressColor(const Vec4f& _color) {
        pressColor_ = _color;
    }

    const Vec4f& getReleaseColor() const {
        return releaseColor_;
    }
    void setReleaseColor(const Vec4f& _color) {
        releaseColor_ = _color;
    }
};
