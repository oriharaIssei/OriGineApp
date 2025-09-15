#include "Stage.h"

/// stl
#include <unordered_map>

/// engine
#define RESOURCE_DIRECTORY
#include "EngineInclude.h"
#include "logger/Logger.h"

#ifdef DEBUG
#include "myGui/MyGui.h"
#include "util/myFileSystem/MyFileSystem.h"
#endif // DEBUG

void Stage::Initialize(GameEntity* /*_entity*/) {
    if (!directory_.empty() && !fileName_.empty()) {
        LoadFile(directory_, fileName_);
    } else {
        LOG_ERROR("Stage::Initialize: Directory or file name is empty.");
    }
}

void Stage::Edit(Scene* /*_scene*/, GameEntity* /*_entity*/, [[maybe_unused]] const std::string& _parentLabel) {
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
#endif
}

void Stage::Finalize() {
    controlPoints_.clear();
    links_.clear();
}

void Stage::SaveFile(const std::string& _directory, const std::string& _filename) {
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
}

void to_json(nlohmann::json& j, const Stage& stage) {
    j["directory"] = stage.directory_;
    j["fileName"]  = stage.fileName_;
}

void from_json(const nlohmann::json& j, Stage& stage) {
    j.at("directory").get_to(stage.directory_);
    j.at("fileName").get_to(stage.fileName_);
}
