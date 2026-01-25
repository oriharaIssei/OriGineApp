#pragma once
#include "system/ISystem.h"

/// stl
#include <string>
#include <vector>

/// <summary>
/// ゲームを開始するまでの処理
/// 順序は　ステージ紹介シーケンス -> スタートタイマー -> ゲーム開始
/// </summary>
class StartSequenceSystem
    : public OriGine::ISystem {
public:
    StartSequenceSystem();
    ~StartSequenceSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(OriGine::EntityHandle _handle) override;

    /// <summary>
    /// ステージ紹介シーケンス の開始処理
    /// </summary>
    void StageIntroductionSequence();
    /// <summary>
    /// スタートタイマー の開始処理
    /// </summary>
    void TimerStartSequence();
    /// <summary>
    /// ゲーム開始(スタートタイマーが終わった時)の処理
    /// </summary>
    void GameStartSequence();

protected:
    bool isIntroductionSequenceEnd_                   = false;
    bool isStarted_                                   = false;
    std::vector<std::string> stopSystems_             = {};
    std::vector<std::string> stopSystemsInStartTimer_ = {};

public:
    bool IsStarted() const {
        return isStarted_;
    }
};
