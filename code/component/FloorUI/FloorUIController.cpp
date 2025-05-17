#include "FloorUIController.h"

// 外部ライブラリなど
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void FloorUIController::Initialize([[maybe_unused]] GameEntity* _entity) {
    // 必要であれば初期化処理をここに記述
}

bool FloorUIController::Edit() {
    bool isChange = false;

#ifdef _DEBUG
    isChange |= CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    ImGui::Text("pos");
    isChange |= DragGuiVectorCommand("startPos", startPos_, 0.01f);
    isChange |= DragGuiVectorCommand("endPos", endPos_, 0.01f);
    isChange |= DragGuiVectorCommand("startScale", startScale_, 0.01f);

    ImGui::Text("easing");
    isChange |= DragGuiCommand("moveEasing.maxTime", moveEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("moveEasing.backRatio", moveEasing_.backRatio, 0.01f);

    isChange |= DragGuiCommand("openEasing_.maxTime", openEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("closeEasing_.maxTime", closeEasing_.maxTime, 0.01f);

#endif // _DEBUG

    return isChange;
}

void to_json(nlohmann::json& j, const FloorUIController& m) {
    j["isAlive"]              = m.isAlive_;
    j["moveEasing.maxTime"]   = m.moveEasing_.maxTime;
    j["startPos"]             = m.startPos_;
    j["endPos"]               = m.endPos_;
    j["moveEasing.backRatio"] = m.moveEasing_.backRatio;
    j["openEasing_.maxTime"]  = m.openEasing_.maxTime;
    j["closeEasing_.maxTime"] = m.closeEasing_.maxTime;
    j["startScale"]           = m.startScale_;
}

void from_json(const nlohmann::json& j, FloorUIController& m) {
    j.at("isAlive").get_to(m.isAlive_);
    j.at("moveEasing.maxTime").get_to(m.moveEasing_.maxTime);
    j.at("startPos").get_to(m.startPos_);
    j.at("endPos").get_to(m.endPos_);
    j.at("moveEasing.backRatio").get_to(m.moveEasing_.backRatio);
    j.at("openEasing_.maxTime").get_to(m.openEasing_.maxTime);
    j.at("closeEasing_.maxTime").get_to(m.closeEasing_.maxTime);
    j.at("startScale").get_to(m.startScale_);
}

void FloorUIController::Finalize() {
    // 必要ならリソース解放などを記述
}
