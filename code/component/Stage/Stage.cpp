#include "Stage.h"

/// stl
#include <unordered_map>

/// engine
#define RESOURCE_DIRECTORY
#include "EngineInclude.h"
#include "logger/Logger.h"
#include "scene/Scene.h"

/// ecs
#include "system/Initialize/CreateStage.h"
#include "system/SystemRunner.h"

#ifdef DEBUG
#include "myGui/MyGui.h"
#endif // DEBUG

float Stage::clearTime_ = 0.f;

void Stage::Initialize(Entity* /*_entity*/) {
    if (!directory_.empty() && !fileName_.empty()) {
        LoadFile(directory_, fileName_);
    } else {
        LOG_ERROR("Stage::Initialize: Directory or file name is empty.");
    }
}

void Stage::Edit([[maybe_unused]] Scene* _scene, Entity* /*_entity*/, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef DEBUG
    ImGui::Text("Directory : %s", directory_.c_str());
    ImGui::Text("FileName : %s", fileName_.c_str());
    ImGui::SameLine();
    std::string label = "SearchFile" + _parentLabel;
    if (ImGui::Button(label.c_str())) {
        // ファイル選択ダイアログを開く
        if (myfs::selectFileDialog(kApplicationResourceDirectory, directory_, fileName_, {"stage"}, true)) {
            directory_ = kApplicationResourceDirectory + "/" + directory_;
            LoadFile(directory_, fileName_);
        }
    }

    // ファイルに変更があったら 読み込み直す
    if (fileWatcher_) {
        if (fileWatcher_->isChanged()) {
            ReloadFile();
            _scene->GetSystemRunnerRef()->UpdateCategory(SystemCategory::Initialize);
        }
    }

#endif // DEBUG
}

void Stage::Finalize() {
    controlPoints_.clear();
    links_.clear();
}

void Stage::SaveFile(const std::string& _directory, const std::string& _filename) {
    // ディレクトリが存在する場合は削除してから保存
    myfs::deleteFile(_directory + "/" + _filename + ".stage");

    BinaryWriter writer(_directory, _filename + ".stage");
    writer.WriteBegin();
    writer.WriteBeginGroup("Stage");
    // 制御点の保存
    writer.WriteBeginGroup("ControlPoints");
    writer.Write("count", static_cast<int32_t>(controlPoints_.size()));
    int32_t index = 0;
    for (const auto& point : controlPoints_) {
        writer.Write<3, float>(std::to_string(index) + "_pos", point.pos_);
        ++index;
    }
    writer.WriteEndGroup();
    // リンクの保存
    writer.WriteBeginGroup("Links");
    writer.Write("count", static_cast<int32_t>(links_.size()));
    index = 0;
    for (const auto& link : links_) {
        writer.Write(std::to_string(index) + "_to", link.to_);
        writer.Write(std::to_string(index) + "_from", link.from_);
        writer.Write<3, float>(std::to_string(index) + "_normal", link.normal_);
        writer.Write(std::to_string(index) + "_height", link.height_);
        writer.Write(std::to_string(index) + "_width", link.width_);
        ++index;
    }
    writer.WriteEndGroup();

    // 開始地点と目標地点のインデックスを保存
    writer.Write("startPointIndex", startPointIndex_);
    writer.Write("goalPointIndex", goalPointIndex_);

    writer.WriteEndGroup();
    writer.WriteEnd();
}
void Stage::LoadFile(const std::string& _directory, const std::string& _filename) {
    BinaryReader reader(_directory, _filename + ".stage");
    if (!reader.ReadFile()) {
        LOG_ERROR("Failed to read stage file: {}", _filename);
        return;
    }

    // 既存のデータをクリア
    controlPoints_.clear();
    links_.clear();

    reader.ReadBeginGroup("Stage");
    // 制御点の読み込み
    reader.ReadBeginGroup("ControlPoints");
    int32_t count = 0;
    reader.Read("count", count);
    ControlPoint point;
    for (size_t i = 0; i < count; ++i) {
        reader.Read<3, float>(std::to_string(i) + "_pos", point.pos_);
        controlPoints_.push_back(point);
    }
    reader.ReadEndGroup();

    // リンクの読み込み
    reader.ReadBeginGroup("Links");
    count = 0;
    reader.Read("count", count);
    Link link;
    for (size_t i = 0; i < count; ++i) {
        std::string indexStr = std::to_string(i);
        reader.Read(indexStr + "_to", link.to_);
        reader.Read(indexStr + "_from", link.from_);
        reader.Read<3, float>(indexStr + "_normal", link.normal_);
        reader.Read(indexStr + "_height", link.height_);
        reader.Read(indexStr + "_width", link.width_);
        links_.push_back(link);
    }
    reader.ReadEndGroup();

    // 開始地点と目標地点のインデックスを読み込み
    reader.Read("startPointIndex", startPointIndex_);
    reader.Read("goalPointIndex", goalPointIndex_);

    reader.ReadEndGroup();

    LOG_INFO("Stage loaded from: {}", _directory + "/" + _filename);
    directory_ = _directory;
    fileName_  = _filename;
#ifndef _RELEASE
    if (fileWatcher_) {
        fileWatcher_->Stop();
        fileWatcher_->SetFilePath(directory_ + "/" + fileName_ + +".stage");
        fileWatcher_->Start();
    } else {
        fileWatcher_ = std::make_shared<FileWatcher>(directory_ + "/" + fileName_ + +".stage");
        fileWatcher_->Start();
    }
#endif // _RELEASE
}

#ifndef _RELEASE
void Stage::ReloadFile() {
    LoadFile(directory_, fileName_);
}
#endif // _RELEASE

void to_json(nlohmann::json& j, const Stage& stage) {
    j["directory"] = stage.directory_;
    j["fileName"]  = stage.fileName_;
}

void from_json(const nlohmann::json& j, Stage& stage) {
    j.at("directory").Get_to(stage.directory_);
    j.at("fileName").Get_to(stage.fileName_);
}
