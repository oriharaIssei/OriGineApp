#pragma once
#include "component/IComponent.h"

/// stl
#include <vector>

/// engine

/// math
#include <math/Vector3.h>

class Stage
    : public IComponent {
public:
    friend class SortControlPointsCommand;

    friend void to_json(nlohmann::json& j, const Stage& stage);
    friend void from_json(const nlohmann::json& j, Stage& stage);

public:
    Stage()           = default;
    ~Stage() override = default;

    void Initialize(GameEntity* _entity);

    void Edit(Scene* _scene, GameEntity* _entity, const std::string& _parentLabel);

    void Finalize();

public:
    struct ControlPoint {
        Vec3f pos_;
    };
    struct Link {
        int32_t to_;
        int32_t from_;

        Vec3f normal_;
        float height_ = 3.f;
        float width_  = 5.f;
    };

private:
    std::vector<ControlPoint> controlPoints_;
    std::vector<Link> links_;

public:
    const std::vector<ControlPoint>& getControlPoints() const { return controlPoints_; }
    const std::vector<Link>& getLinks() const { return links_; }
    std::vector<ControlPoint>& getControlPointsRef()  { return controlPoints_; }
    std::vector<Link>& getLinksRef()  { return links_; }
    /// <summary>
    /// すべての制御点を削除
    /// </summary>
    void clearControlPoints() { controlPoints_.clear(); }
    /// <summary>
    /// すべてのリンクを削除
    /// </summary>
    void clearLinks() { links_.clear(); }

    void addControlPoint(const ControlPoint& point) {
        controlPoints_.emplace_back(point);
    }
    void addLink(const Link& link) {
        links_.emplace_back(link);
    }

    void removeControlPoint(int32_t index) {
        if (index < 0 || index >= controlPoints_.size()) {
            return;
        }
        controlPoints_.erase(controlPoints_.begin() + index);
    }
    void removeLink(int32_t index) {
        if (index < 0 || index >= links_.size()) {
            return;
        }
        links_.erase(links_.begin() + index);
    }

#ifdef DEBUG
private:


#endif // DEBUG
};
