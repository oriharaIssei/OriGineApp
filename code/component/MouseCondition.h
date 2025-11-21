#pragma once
#include "component/IComponent.h"

/// math
#include "math/Vector2.h"

/// <summary>
/// マウスの状態を指定するコンポーネント
/// </summary>
class MouseCondition
    : public IComponent {
    friend void to_json(nlohmann::json& j, const MouseCondition& c);
    friend void from_json(const nlohmann::json& j, MouseCondition& c);

public:
    MouseCondition();
    ~MouseCondition() override;

    void Initialize(Entity* _entity) override;
    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
    void Finalize() override;

private:
    bool isActive_      = true;
    bool isShowCursor_  = true;
    bool isFixCursor_   = false;
    Vec2f fixCursorPos_ = Vec2f(0.0f, 0.0f);

public:
    bool IsActive() const { return isActive_; }
    bool isShowCursor() const { return isShowCursor_; }
    bool isFixCursor() const { return isFixCursor_; }
    Vec2f fixCursorPos() const { return fixCursorPos_; }
};
