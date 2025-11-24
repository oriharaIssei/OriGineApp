#include "PlayerProgressStore.h"

/// stl
#include <algorithm>

void PlayerProgressStore::Initialize() {
    progressDataMap_.clear();
}

void PlayerProgressStore::Finalize() {
    progressDataMap_.clear();
}

void PlayerProgressStore::StageCleared(int32_t _stageNum, int32_t _difficulty, float _clearTime) {
    // 進行状況データを更新する
    auto itr = progressDataMap_.find(_stageNum);
    if (itr != progressDataMap_.end()) {
        PlayerProgressData& existingData = itr->second;
        // 既存のデータがある場合、クリアタイムを比較して更新する
        if (_clearTime < existingData.clearTime || !existingData.isCleared) {
            existingData.clearTime = _clearTime;
        }
        existingData.isCleared = true;
        return;
    }

    // データが存在しない場合、新しいデータを追加する
    PlayerProgressData newData;
    newData.stageNumber   = _stageNum;
    newData.difficulty    = _difficulty;
    newData.isCleared     = true;
    newData.clearTime     = _clearTime;

    progressDataMap_.emplace(_stageNum, newData);
}
