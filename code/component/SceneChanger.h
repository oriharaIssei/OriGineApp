#pragma once
/// parent
#include "component/IComponent.h"

/// stl
#include <string>

class SceneChanger
    : public IComponent {
    friend void to_json(nlohmann::json& j, const SceneChanger& r);
    friend void from_json(const nlohmann::json& j, SceneChanger& r);

public:
    SceneChanger();
    virtual ~SceneChanger();

    virtual void Initialize(GameEntity* _entity) override;
    virtual bool Edit() override;

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
    void setNextSceneName(const std::string& _nextSceneName) { nextSceneName_ = _nextSceneName; }
    const std::string& getNextSceneName() const { return nextSceneName_; }
};
