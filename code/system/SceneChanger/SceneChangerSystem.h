#pragma once

#include "sceneManager/SceneManager.h"
#include "system/ISystem.h"

class SceneChangerStatus;
class TimerStatus;
class Input;
class SceneChangerSystem
    : public ISystem {
public:
    SceneChangerSystem();
    ~SceneChangerSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

    void ChangeResultToGame();
    void ChangeTitleToResult();
    void ChangeGameToTitle();
    void ChangeTitleToGame();

    void ChangeSceneTitle();
    void ChangeSceneResult();
    void ChangeSceneGame();


protected:
    virtual void UpdateEntity(GameEntity* _entity) override;

private:
    Input* input_                = nullptr;
    SceneChangerStatus* gameEnd_ = nullptr;
    SceneManager* sceneManager_  = nullptr;
};
