#pragma once

/// stl
#include <cstdint>
#include <unordered_map>

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
    void StageCleared(int32_t _stageNum, int32_t _difficulty, float _clearTime);

private:
    PlayerProgressStore()                                      = default;
    ~PlayerProgressStore()                                     = default;
    PlayerProgressStore(const PlayerProgressStore&)            = delete;
    PlayerProgressStore& operator=(const PlayerProgressStore&) = delete;

private:
    int32_t lastPlayStageNumber_ = -1; // 最後にプレイしたステージ番号
    PlayerProgressData lastPlayStageProgressData_; // 最後にプレイしたステージの進行状況データ

    std::unordered_map<int32_t, PlayerProgressData> progressDataMap_; // 進行状況データリスト(clearTimeはbestTimeを残す)

public:
    int32_t GetLastPlayNumber() const { return lastPlayStageNumber_; }
    const std::unordered_map<int32_t, PlayerProgressData>& GetProgressDataMap() { return progressDataMap_; }

    const PlayerProgressData& GetProgressData(int32_t _stageNumber) {
        return progressDataMap_.at(_stageNumber);
    }
    const PlayerProgressData& GetLastPlayStageProgressData() const {
        return lastPlayStageProgressData_;
    }
};
