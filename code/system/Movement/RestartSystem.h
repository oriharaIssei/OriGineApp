#pragma once
#include "system/ISystem.h"

/// stl
#include <string>
#include <vector>

/// <summary>
///  リスタートメッセージを受け取ってリスタート処理を行うシステム
/// </summary>
class RestartSystem
    : public OriGine::ISystem {
public:
    RestartSystem();
    ~RestartSystem() override;

    void Initialize() override;
    void Finalize() override;

protected:
    void Update() override;

protected:
    size_t restartEventId_ = 0;
    bool isRestarting_     = false;
};
