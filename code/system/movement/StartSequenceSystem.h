#pragma once
#include "system/ISystem.h"

/// stl
#include <string>
#include <vector>

/// <summary>
/// ゲームを開始するまでの処理
/// </summary>
class StartSequenceSystem
    : public OriGine::ISystem {
public:
    StartSequenceSystem();
    ~StartSequenceSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(OriGine::Entity* _entity) override;

    /// <summary>
    /// スタートタイマー の開始処理
    /// </summary>
    void TimerStartSequence();
    /// <summary>
    /// ゲーム開始(スタートタイマーが終わった時)の処理
    /// </summary>
    void GameStartSequence();

protected:
    bool isStarted_                       = false;
    std::vector<std::string> stopSystems_ = {};

public:
    bool IsStarted() const {
        return isStarted_;
    }
};
