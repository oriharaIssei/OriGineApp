#pragma once

/// stl
#include <string>
#include <unordered_map>
/// math
#include <cstdint>

/// engine
#include "debugReplayer/ReplayPlayer.h"
#include "debugReplayer/ReplayRecorder.h"

struct PlayerProgressData {
    int32_t stageNumber = -1; // ステージ番号
    int32_t difficulty  = -1; // 難易度
    bool isCleared      = false; // クリアしたかどうか
    float clearTime     = 0.0f; // クリアタイム
};

/// <summary>
/// Playerの進行状況を管理するストア
/// </summary>
class PlayerProgressStore {
public:
    static PlayerProgressStore* GetInstance() {
        static PlayerProgressStore instance;

        if (!instance.isInitialized_) {
            instance.Initialize();
            instance.isInitialized_ = true;
        }

        return &instance;
    }

    void Initialize();
    void Finalize();

    /// <summary>
    /// ステージプレイ開始時の処理
    /// </summary>
    /// <param name="_stage"></param>
    void StagePlayStart(int32_t _stage) {
        lastPlayStageNumber_ = _stage;
    }
    /// <summary>
    /// ステージクリア時の処理
    /// </summary>
    /// <param name="_stageNum">ステージ番号</param>
    /// <param name="_difficulty">難易度</param>
    /// <param name="_clearTime">クリアした時間</param>
    void StageCleared(OriGine::ReplayRecorder* _recorder, int32_t _stageNum, int32_t _difficulty, float _clearTime);

    /// <summary>
    /// 進行状況データの読み込み
    /// </summary>
    void LoadProgressData();
    /// <summary>
    /// 進行状況データの保存
    /// </summary>
    void SaveProgressData();

    /// <summary>
    /// ベストプレイデータの読み込み
    /// </summary>
    bool LoadBestPlayData(int32_t _stageNum, OriGine::ReplayPlayer* _replayer);
    /// <summary>
    /// ベストプレイデータの保存
    /// </summary>
    /// <param name="_stageNum"></param>
    /// <param name="_recorder"></param>
    void SaveBestPlayData(int32_t _stageNum, OriGine::ReplayRecorder* _recorder);

private:
    PlayerProgressStore()                                      = default;
    ~PlayerProgressStore()                                     = default;
    PlayerProgressStore(const PlayerProgressStore&)            = delete;
    PlayerProgressStore& operator=(const PlayerProgressStore&) = delete;

private:
    bool isInitialized_                    = false;
    const std::string kProgressDataFolder_ = "ProgressData/";
    const std::string kBestPlayDataFolder_ = "BestPlayData"; //  kProgressDataFolder_ の下に保存する
    const char* kProgressDataFileName_     = "PlayerProgressData.json";

    int32_t lastPlayStageNumber_ = -1; // 最後にプレイしたステージ番号
    PlayerProgressData lastPlayStageProgressData_; // 最後にプレイしたステージの進行状況データ

    std::unordered_map<int32_t, PlayerProgressData> progressDataMap_; // 進行状況データリスト(clearTimeはbestTimeを残す)

public:
    int32_t GetLastPlayNumber() const { return lastPlayStageNumber_; }
    const std::unordered_map<int32_t, PlayerProgressData>& GetProgressDataMap() { return progressDataMap_; }

    bool HasProgressData(int32_t _stageNumber) const {
        return progressDataMap_.find(_stageNumber) != progressDataMap_.end();
    }

    const PlayerProgressData& GetProgressData(int32_t _stageNumber) {
        return progressDataMap_.at(_stageNumber);
    }
    const PlayerProgressData& GetLastPlayStageProgressData() const {
        return lastPlayStageProgressData_;
    }
};
