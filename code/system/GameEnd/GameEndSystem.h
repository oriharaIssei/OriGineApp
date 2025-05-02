#pragma once

#include "system/ISystem.h"
#include"sceneManager/SceneManager.h"

class GameEndSystem
    : public ISystem {
public:
    GameEndSystem();
    ~GameEndSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

   

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;

    private:
    SceneManager* sceneManager_ = nullptr;
   
};
