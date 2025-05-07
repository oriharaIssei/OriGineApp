#pragma once

#include "system/ISystem.h"
#include"sceneManager/SceneManager.h"

class GameEnd;
class TimerStatus;
class GameEndSystem
    : public ISystem {
public:
    GameEndSystem();
    ~GameEndSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

    void AnimationChangeGameToResult();

    void ChangeSceneTitle();
    void ChangeSceneResult();
    void ChangeSceneGame();

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;

    private:
    GameEnd* gameEnd_=nullptr;
    SceneManager* sceneManager_ = nullptr;
   
};
