#pragma once
#include "component/IComponent.h"

/// stl
#include <vector>

/// engine

/// math
#include <math/Vector3.h>

class Stage
    : public IComponent {
    friend class SortControlPointsCommand;

public:
    Stage()           = default;
    ~Stage() override = default;

    void Initialize(GameEntity* _entity);

    bool Edit();

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
    /// <summary>
    /// first = control point index , second = ControlPoint
    /// </summary>
    std::vector<std::pair<int32_t, ControlPoint>> editControlPoints_;
    std::vector<std::pair<int32_t, Link>> editLinks_;

    Vec2f clickedMousePos_;
    Vec2f releasedMousePos_;

public:
    const std::vector<std::pair<int32_t, ControlPoint>>& getEditControlPoints() const {
        return editControlPoints_;
    }
    const std::vector<std::pair<int32_t, Link>>& getEditLinks() const {
        return editLinks_;
    }

    void addEditControlPoint(int32_t index) {
        editControlPoints_.emplace_back(index, controlPoints_[index]);
    }
    void addEditLink(int32_t index) {
        editLinks_.emplace_back(index, links_[index]);
    }

    void clearEditControlPoints() { editControlPoints_.clear(); }
    void clearEditLinks() { editLinks_.clear(); }

    void removeEditControlPoint(int32_t index) {
        editControlPoints_.erase(
            std::remove_if(editControlPoints_.begin(), editControlPoints_.end(),
                [index](const auto& p) { return p.first == index; }),
            editControlPoints_.end());
    }
    void removeEditLink(int32_t index) {
        editLinks_.erase(
            std::remove_if(editLinks_.begin(), editLinks_.end(),
                [index](const auto& p) { return p.first == index; }),
            editLinks_.end());
    }

#endif // DEBUG
};
