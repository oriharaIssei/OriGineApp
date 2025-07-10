#pragma once

/// parent
#include "FrameWork.h"

/// Engine
class SceneManager;

class MyGame
    : public FrameWork {
public:
    MyGame();
    ~MyGame();

    void Initialize() override;
    void Finalize() override;

    void Run();


private:
    SceneManager* sceneManager_ = nullptr;
};
