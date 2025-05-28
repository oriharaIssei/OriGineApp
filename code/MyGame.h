#pragma once

/// parent
#include "FrameWork.h"

/// Engine
class Engine;
class GlobalVariables;
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
    /// <summary>
    /// 使用される コンポーネントを登録する
    /// </summary>
    void RegisterUsingComponents();
    /// <summary>
    /// 使用される System を登録する
    /// </summary>
    void RegisterUsingSystems();

        void ResourceAllLoad();

private:
    Engine* engine_             = nullptr;
    GlobalVariables* variables_ = nullptr;
    SceneManager* sceneManager_ = nullptr;
};
