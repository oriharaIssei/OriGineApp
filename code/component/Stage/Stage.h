#pragma once
#include "component/IComponent.h"

/// stl
#include <memory>
#include <vector>

/// util
#include "binaryIO/BinaryIO.h"
#include "myFileSystem/MyFileSystem.h"

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

private:
    static float clearTime_; // クリアタイム（秒）
public:
    static float GetClearTime() { return clearTime_; }
    static void SetClearTime(float time) { clearTime_ = time; }

public:
    Stage()           = default;
    ~Stage() override = default;

    void Initialize(Entity* _entity);

    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel);

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

    std::vector<ControlPoint>
        controlPoints_;
    std::vector<Link> links_;
    int32_t startPointIndex_ = -1; // 開始地点のインデックス
    int32_t goalPointIndex_  = -1; // 目標地点のインデックス
public:
    const std::vector<ControlPoint>& GetControlPoints() const { return controlPoints_; }
    const std::vector<Link>& GetLinks() const { return links_; }
    std::vector<ControlPoint>& GetControlPointsRef() { return controlPoints_; }
    std::vector<Link>& GetLinksRef() { return links_; }

    const std::string& GetDirectory() const { return directory_; }
    void SetDirectory(const std::string& dir) { directory_ = dir; }
    const std::string& GetFileName() const { return fileName_; }
    void SetFileName(const std::string& name) { fileName_ = name; }

    int32_t GetStartPointIndex() const { return startPointIndex_; }
    void SetStartPointIndex(int32_t index) {
        if (index < 0 || index >= controlPoints_.size()) {
            return;
        }
        startPointIndex_ = index;
    }

    int32_t GetGoalPointIndex() const { return goalPointIndex_; }
    void SetGoalPointIndex(int32_t index) {
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
    void insertControlPoint(int32_t _index, const ControlPoint& point) {
        controlPoints_.insert(controlPoints_.begin() + _index, point);
    }
    void addLink(const Link& link) {
        links_.emplace_back(link);
    }
    void insertLink(int32_t _index, const Link& link) {
        links_.insert(links_.begin() + _index, link);
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

#ifndef _RELEASE
    FileWatcher* GetFileWatcher() { return fileWatcher_.get(); }
#endif // DEBUG
};
