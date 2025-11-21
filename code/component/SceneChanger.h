#pragma once
/// parent
#include "component/IComponent.h"

/// stl
#include <string>

/// <summary>
/// シーンを変更するコンポーネント
/// </summary>
class SceneChanger
    : public IComponent {
    friend void to_json(nlohmann::json& j, const SceneChanger& r);
    friend void from_json(const nlohmann::json& j, SceneChanger& r);

public:
    SceneChanger();
    virtual ~SceneChanger();

    virtual void Initialize(Entity* _entity) override;
    virtual void Edit(Scene* _scene, Entity* _entity,  const std::string& _parentLabel) override;

    virtual void Finalize() override;

    /// <summary>
    /// SceneTransitionSystem を使用する場合に呼び出す
    /// </summary>
    void ChangeScene() {
        isSceneChanged_ = true;
    }
private:
    bool isSceneChanged_ = false;
    std::string nextSceneName_;

public:
    bool isChanged() const { return isSceneChanged_; }
    void SetNextSceneName(const std::string& _nextSceneName) { nextSceneName_ = _nextSceneName; }
    const std::string& GetNextSceneName() const { return nextSceneName_; }
};
