#pragma once

/// parent
#include "FrameWork.h"

/// stl
#include <memory>

/// Engine
class SceneManager;
class ReplayRecorder;

/// <summary>
/// ゲーム本体
/// </summary>
class MyGame
    : public FrameWork {
public:
    MyGame();
    ~MyGame();

    void Initialize(const std::vector<std::string>& _commandLines) override;
    void Finalize() override;

    void Run();

private:
    SceneManager* sceneManager_ = nullptr;

#ifdef _DEVELOP
    bool isRecording_                         = true;
    std::unique_ptr<ReplayRecorder> recorder_ = nullptr;
#endif // _DEVELOP
};
