#pragma once
#include "component/IComponent.h"

/// stl
#include <vector>

/// util
#include "binaryIO/BinaryIO.h"

/// math
#include <math/Vector3.h>

/// <summary>
/// ステージを構成する情報
/// </summary>
class Stage
    : public IComponent {
public:
    friend class SortControlPointsCommand;

    friend void to_json(nlohmann::json& j, const Stage& stage);
    friend void from_json(const nlohmann::json& j, Stage& stage);

    static constexpr float kObjectMargin = 1.f; // オブジェクト生成時, toとfromにプラスされる余白

public:
    Stage()           = default;
    ~Stage() override = default;

    void Initialize(GameEntity* _entity);

    void Edit(Scene* _scene, GameEntity* _entity, const std::string& _parentLabel);

    void Finalize();

    void SaveFile(const std::string& _directory, const std::string& _filename);
    void LoadFile(const std::string& _directory, const std::string& _filename);

public:
    struct ControlPoint {
        Vec3f pos_;
    };
    struct Link {
        int32_t to_;
        int32_t from_;

        Vec3f normal_ = {0.f, 1.f, 0.f};
        float height_ = 3.f;
        float width_  = 22.f;
    };

private:
    std::string directory_;
    std::string fileName_;

    std::vector<ControlPoint> controlPoints_;
    std::vector<Link> links_;
    int32_t startPointIndex_ = -1; // 開始地点のインデックス
    int32_t goalPointIndex_  = -1; // 目標地点のインデックス
public:
    const std::vector<ControlPoint>& getControlPoints() const { return controlPoints_; }
    const std::vector<Link>& getLinks() const { return links_; }
    std::vector<ControlPoint>& getControlPointsRef() { return controlPoints_; }
    std::vector<Link>& getLinksRef() { return links_; }

    const std::string& getDirectory() const { return directory_; }
    const std::string& getFileName() const { return fileName_; }

    int32_t getStartPointIndex() const { return startPointIndex_; }
    void setStartPointIndex(int32_t index) {
        if (index < 0 || index >= controlPoints_.size()) {
            return;
        }
        startPointIndex_ = index;
    }

    int32_t getGoalPointIndex() const { return goalPointIndex_; }
    void setGoalPointIndex(int32_t index) {
        if (index < 0 || index >= controlPoints_.size()) {
            return;
        }
        goalPointIndex_ = index;
    }

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
};
