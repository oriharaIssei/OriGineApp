#pragma once

#include <string>

class Engine;
class GlobalVariables;

class FrameWork {
public:
    FrameWork();
    virtual ~FrameWork();

    virtual void Initialize(const std::string& _commandLine) = 0;
    virtual void Finalize()   = 0;

    virtual void Run() = 0;

protected:
    bool isEndRequest_          = false;
    Engine* engine_             = nullptr;
    GlobalVariables* variables_ = nullptr;
};

/// <summary>
/// 使用される コンポーネントを登録する
/// </summary>
void RegisterUsingComponents();
/// <summary>
/// 使用される System を登録する
/// </summary>
void RegisterUsingSystems();
