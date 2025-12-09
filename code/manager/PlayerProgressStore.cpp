#include "PlayerProgressStore.h"

/// stl
#include <algorithm>

#include <fstream>

/// engine
#define RESOURCE_DIRECTORY
#include "EngineInclude.h"

/// util
#include "myFileSystem/MyFileSystem.h"

/// externals
#include <nlohmann/json.hpp>

void PlayerProgressStore::Initialize() {
    progressDataMap_.clear();
}

void PlayerProgressStore::Finalize() {
    progressDataMap_.clear();
}

void PlayerProgressStore::StageCleared(ReplayRecorder* _recorder, int32_t _stageNum, int32_t _difficulty, float _clearTime) {
    lastPlayStageNumber_ = _stageNum;

    PlayerProgressData newData;
    newData.stageNumber = _stageNum;
    newData.difficulty  = _difficulty;
    newData.isCleared   = true;
    newData.clearTime   = _clearTime;

    lastPlayStageProgressData_ = newData;

    // 進行状況データを更新する
    auto itr = progressDataMap_.find(_stageNum);
    if (itr != progressDataMap_.end()) {
        PlayerProgressData& existingData = itr->second;
        existingData.isCleared           = true;
        // 既存のデータがある場合、クリアタイムを比較して更新する
        if (_clearTime > existingData.clearTime || !existingData.isCleared) {
            existingData.clearTime = _clearTime;
            if (_recorder) {
                SaveBestPlayData(_stageNum, _recorder);
                SaveProgressData();
            }
        }
        return;
    }

    // データが存在しない場合、新しいデータを追加する
    progressDataMap_.emplace(_stageNum, newData);

    SaveProgressData();
    if (_recorder) {
        SaveBestPlayData(_stageNum, _recorder);
    }
}

void PlayerProgressStore::LoadProgressData() {
    std::string filePath = kApplicationResourceDirectory + "/" + kProgressDataFolder_ + kProgressDataFileName_;
    std::ifstream ifs(filePath);
    if (!ifs) {
        return;
    }
    nlohmann::json jsonData;
    ifs >> jsonData;
    ifs.close();
    int32_t stageCount = jsonData["StageCount"];
    auto& stagesJson   = jsonData["Stages"];
    // ステージごとの進行状況データを読み込む
    for (const auto& stageData : stagesJson) {
        PlayerProgressData progressData;
        progressData.stageNumber = stageData["StageNumber"];
        progressData.difficulty  = stageData["Difficulty"];
        progressData.isCleared   = stageData["IsCleared"];
        progressData.clearTime   = stageData["ClearTime"];
        progressDataMap_.emplace(progressData.stageNumber, progressData);
    }
}

void PlayerProgressStore::SaveProgressData() {
    nlohmann::json jsonData = nlohmann::json::object();
    jsonData["StageCount"]  = progressDataMap_.size();
    jsonData["Stages"]      = nlohmann::json::array();
    auto& stagesJson        = jsonData["Stages"];
    // ステージごとの進行状況データを読み込む
    for (const auto& [stageNumber, progressData] : progressDataMap_) {
        nlohmann::json stageData = nlohmann::json::object();
        stageData["StageNumber"] = progressData.stageNumber;
        stageData["Difficulty"]  = progressData.difficulty;
        stageData["IsCleared"]   = progressData.isCleared;
        stageData["ClearTime"]   = progressData.clearTime;
        stagesJson.push_back(stageData);
    }

    // JSONファイルに書き込み
    std::string filePath = kApplicationResourceDirectory + "/" + kProgressDataFolder_ + kProgressDataFileName_;
    myFs::CreateFolder(kApplicationResourceDirectory + "/" + kProgressDataFolder_);
    myFs::DeleteMyFile(filePath); // 古いファイルを削除してから保存
    std::ofstream ofs(filePath);
    if (!ofs) {
        return;
    }
    ofs << std::setw(4) << jsonData << std::endl;
    ofs.close();
}

bool PlayerProgressStore::LoadBestPlayData(int32_t _stageNum, ReplayPlayer* _replayer) {
    std::string stageFilePath = kApplicationResourceDirectory + "/" + kBestPlayDataFolder_ + "/Stage_" + std::to_string(_stageNum) + '.' + kReplayFileExtension;
    return _replayer->LoadFromFile(stageFilePath);
}

void PlayerProgressStore::SaveBestPlayData(int32_t _stageNum, ReplayRecorder* _recorder) {
    std::string directory = kApplicationResourceDirectory + "/" + kBestPlayDataFolder_;
    std::string filename  = "Stage_" + std::to_string(_stageNum);
    myFs::CreateFolder(directory);
    myFs::DeleteMyFile(directory + "/" + filename); // 古いファイルを削除してから保存

    _recorder->SaveToFile(directory, filename);
}
