#include "Stage.h"

/// stl
#include <unordered_map>

/// engine
#define RESOURCE_DIRECTORY
#include "EngineInclude.h"

#ifdef DEBUG
#include "myGui/MyGui.h"
#include "util/myFileSystem/MyFileSystem.h"
#endif // DEBUG

void Stage::Initialize(GameEntity* /*_entity*/) {
    if (!directory_.empty() && !directory_.empty()) {
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
    std::string label = "SearchFile##" + _parentLabel;
    if (ImGui::Button(label.c_str())) {
        // ファイル選択ダイアログを開く
        if (myfs::selectFileDialog(kApplicationResourceDirectory, directory_, fileName_, {"stage"})) {
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
    for (const auto& point : controlPoints_) {
        writer.Write<3, float>("pos", point.pos_);
    }
    writer.WriteEndGroup();
    // リンクの保存
    writer.WriteBeginGroup("Links");
    for (const auto& link : links_) {
        writer.Write("to", link.to_);
        writer.Write("from", link.from_);
        writer.Write<3, float>("normal", link.normal_);
        writer.Write("height", link.height_);
        writer.Write("width", link.width_);
    }
    writer.WriteEndGroup();
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
    ControlPoint point;
    while (true) {
        try {
            reader.Read<3, float>("pos", point.pos_);
            controlPoints_.push_back(point);
        } catch (const std::exception& e) {
            break; // 例外が発生したら終了
        }
    }
    reader.ReadEndGroup();

    // リンクの読み込み
    reader.ReadBeginGroup("Links");
    Link link;
    while (true) {
        try {
            reader.Read("to", link.to_);
            reader.Read("from", link.from_);
            reader.Read<3, float>("normal", link.normal_);
            reader.Read("height", link.height_);
            reader.Read("width", link.width_);
            links_.push_back(link);
        } catch (const std::exception& e) {
            break; // 例外が発生したら終了
        }
    }
    reader.ReadEndGroup();

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
