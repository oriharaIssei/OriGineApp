#pragma once
/// parent
#include "component/IComponent.h"

/// stl
#include <vector>

/// engine
#include "input/GamePadInput.h"
#include "input/KeyboardInput.h"

// component
#include "component/renderer/Sprite.h"

/// math
#include <stdint.h>
#include <Vector4.h>

/// <summary>
/// Sprite を Button として扱うためのクラス
/// </summary>
class Button
    : public IComponent {
    friend void to_json(nlohmann::json& j, const Button& r);
    friend void from_json(const nlohmann::json& j, Button& r);

public:
    Button();
    ~Button();

    void Initialize(Entity* _entity);

    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel);

    void Finalize();

private:
    bool isHovered_  = false;
    bool isPressed_  = false;
    bool IsReleased_ = false;

    std::vector<PadButton> shortcutPadButton_;
    std::vector<Key> shortcutKey_;

    Vec4f normalColor_  = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    Vec4f hoverColor_   = Vec4f(0.8f, 0.8f, 0.8f, 1.0f);
    Vec4f pressColor_   = Vec4f(0.6f, 0.6f, 0.6f, 1.0f);
    Vec4f releaseColor_ = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);

public: // Getter Setter
    const std::vector<PadButton>& GetShortcutPadButton() const {
        return shortcutPadButton_;
    }
    const std::vector<Key>& GetShortcutKey() const {
        return shortcutKey_;
    }

    bool isHovered() const {
        return isHovered_;
    }
    void SetHovered(bool _hovered) {
        isHovered_ = _hovered;
    }

    bool isPressed() const {
        return isPressed_;
    }
    void SetPressed(bool _pressed) {
        isPressed_ = _pressed;
    }

    bool IsReleased() const {
        return IsReleased_;
    }
    void SetReleased(bool _released) {
        IsReleased_ = _released;
    }

    const Vec4f& GetNormalColor() const {
        return normalColor_;
    }
    void SetNormalColor(const Vec4f& _color) {
        normalColor_ = _color;
    }

    const Vec4f& GetHoverColor() const {
        return hoverColor_;
    }
    void SetHoverColor(const Vec4f& _color) {
        hoverColor_ = _color;
    }

    const Vec4f& GetPressColor() const {
        return pressColor_;
    }
    void SetPressColor(const Vec4f& _color) {
        pressColor_ = _color;
    }

    const Vec4f& GetReleaseColor() const {
        return releaseColor_;
    }
    void SetReleaseColor(const Vec4f& _color) {
        releaseColor_ = _color;
    }
};
