#pragma once
#include "component/IComponent.h"

/// stl
#include <memory>
#include <vector>

/// util
#include "binaryIO/BinaryIO.h"
#include "myFileSystem/MyFileSystem.h"

/// math
#include <math/Quaternion.h>
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

private:
    static float clearTime_; // クリアタイム（秒）
public:
    static float getClearTime() { return clearTime_; }
    static void setClearTime(float time) { clearTime_ = time; }

public:
    Stage()           = default;
    ~Stage() override = default;

    void Initialize(GameEntity* _entity);

    void Edit(Scene* _scene, GameEntity* _entity, const std::string& _parentLabel);

    void Finalize();

    void SaveFile(const std::string& _directory, const std::string& _filename);
    void LoadFile(const std::string& _directory, const std::string& _filename);

#ifndef _RELEASE
    void ReloadFile();
#endif // DEBUG
public:
    struct ControlPoint {
        Vec3f pos_;
    };
    struct Step {
        int32_t pointIndex_;
        Vec3f size_        = {22.f, 1.2f, 0.8f};
        Quaternion rotate_ = Quaternion::Identity();
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

#ifndef _RELEASE
    std::shared_ptr<FileWatcher> fileWatcher_ = nullptr;
#endif // DEBUG

    std::vector<ControlPoint> controlPoints_;
    std::vector<Link> links_;
    std::vector<Step> steps_;
    int32_t startPointIndex_ = -1; // 開始地点のインデックス
    int32_t goalPointIndex_  = -1; // 目標地点のインデックス
public:
    const std::vector<ControlPoint>& getControlPoints() const { return controlPoints_; }
    const std::vector<Link>& getLinks() const { return links_; }
    const std::vector<Step>& getSteps() const { return steps_; }
    std::vector<ControlPoint>& getControlPointsRef() { return controlPoints_; }
    std::vector<Link>& getLinksRef() { return links_; }
    std::vector<Step>& getStepsRef() { return steps_; }

    const std::string& getDirectory() const { return directory_; }
    void setDirectory(const std::string& dir) { directory_ = dir; }
    const std::string& getFileName() const { return fileName_; }
    void setFileName(const std::string& name) { fileName_ = name; }

    int32_t getStartPointIndex() const { return startPointIndex_; }
    void setStartPointIndex(int32_t index);

    int32_t getGoalPointIndex() const { return goalPointIndex_; }
    void setGoalPointIndex(int32_t index);

    /// <summary>
    /// すべての制御点を削除
    /// </summary>
    void clearControlPoints() { controlPoints_.clear(); }
    /// <summary>
    /// すべてのリンクを削除
    /// </summary>
    void clearLinks() { links_.clear(); }

    void clearSteps() { steps_.clear(); }

    void addControlPoint(const ControlPoint& _point) {
        controlPoints_.emplace_back(_point);
    }
    void insertControlPoint(int32_t _index, const ControlPoint& _point) {
        controlPoints_.insert(controlPoints_.begin() + _index, _point);
    }
    void addLink(const Link& _link) {
        links_.emplace_back(_link);
    }
    void insertLink(int32_t _index, const Link& _link) {
        links_.insert(links_.begin() + _index, _link);
    }
    void addStep(const Step& _step) {
        steps_.emplace_back(_step);
    }
    void insertStep(int32_t _index, const Step& _step) {
        steps_.insert(steps_.begin() + _index, _step);
    }

    void removeControlPoint(int32_t _index) {
        if (_index < 0 || _index >= controlPoints_.size()) {
            return;
        }
        controlPoints_.erase(controlPoints_.begin() + _index);
    }
    void removeLink(int32_t _index) {
        if (_index < 0 || _index >= links_.size()) {
            return;
        }
        links_.erase(links_.begin() + _index);
    }
    void removeStep(int32_t _index) {
        if (_index < 0 || _index >= steps_.size()) {
            return;
        }
        links_.erase(links_.begin() + _index);
    }

#ifndef _RELEASE
    FileWatcher* getFileWatcher() { return fileWatcher_.get(); }
#endif // DEBUG
};
