#pragma once
/// parent
#include "component/IComponent.h"

/// stl
#include <string>

class SceneChanger
    : public IComponent {
public:
    SceneChanger();
    virtual ~SceneChanger();

    virtual void Initialize(GameEntity* _entity) override;
    virtual bool Edit() override;
    virtual void Save(BinaryWriter& _writer) override;
    virtual void Load(BinaryReader& _reader) override;

    virtual void Finalize() override;

private:
    std::string nextSceneName_;

public:
    void setNextSceneName(const std::string& _nextSceneName) { nextSceneName_ = _nextSceneName; }
    const std::string& getNextSceneName() const { return nextSceneName_; }
};
