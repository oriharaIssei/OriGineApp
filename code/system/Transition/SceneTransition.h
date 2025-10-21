#pragma once
#include "system/ISystem.h"

/// <summary>
/// シーン遷移システム(エフェクトの再生管理など)
/// </summary>
class SceneTransition
    : public ISystem {
public:
    SceneTransition();
    ~SceneTransition() override;

    virtual void Initialize();
    // virtual void Update();
    // virtual void Edit();
    virtual void Finalize();

    virtual void Update() override;

protected:
    virtual void UpdateEntity(Entity* _entity) override;

    void EnterSceneUpdate();
    void ExitSceneUpdate();

protected:
    bool enterScene_ = true;
    bool exitScene_  = false;

    const float maxTransitionTime_ = 0.8f; // 最大遷移時間
    float currentTransitionTime_   = 0.0f; // 現在の遷移時間

    int32_t usingEntityId_           = -1;
    int32_t sceneChangerComponentId_ = -1;
};
