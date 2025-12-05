#pragma once
#include "system/ISystem.h"

/// stl
#include <string>
#include <vector>

/// <summary>
/// ゲームを開始するまでの処理
/// </summary>
class StartSequenceSystem
    : public ISystem {
public:
    StartSequenceSystem();
    ~StartSequenceSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* _entity) override;

    /// <summary>
    /// スタートタイマー の開始処理
    /// </summary>
    void TimerStartSeqence();
    /// <summary>
    /// ゲーム開始(スタートタイマーが終わった時)の処理
    /// </summary>
    void GameStartSeqence();

protected:
    bool isStarted_                       = false;
    std::vector<std::string> stopSystems_ = {};
};
