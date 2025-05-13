#include "TutorialArrowStatus.h"

// 外部ライブラリなど
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void TutorialArrowStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    // 必要であれば初期化処理をここに記述
}

bool TutorialArrowStatus::Edit() {
    bool isChange = false;

    isChange |= CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    ImGui::Text("pos");
    isChange |= DragGuiVectorCommand("startPos", startPos_, 0.01f);
    isChange |= DragGuiVectorCommand("endPos", endPos_, 0.01f);

    ImGui::Text("easing");
    isChange |= DragGuiCommand("moveEasing.maxTime", moveEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("moveEasing.backRatio", moveEasing_.backRatio, 0.01f);

    ImGui::Text("etc");
    // ComboDigit（桁数）のUI選択
    static const char* directionLabel[] = {
        "LEFT",
        "RIGHT"};

    int currentIndex = static_cast<int>(arrowDirection_);
    if (ImGui::Combo("Arrow Direction", &currentIndex, directionLabel, static_cast<int>(ArrowDirection::COUNT))) {
        auto command    = std::make_unique<SetterCommand<ArrowDirection>>(&arrowDirection_, static_cast<ArrowDirection>(currentIndex));
        EditorGroup::getInstance()->pushCommand(std::move(command));
    }
    return isChange;
}

void to_json(nlohmann::json& j, const TutorialArrowStatus& m) {
    j["isAlive"]              = m.isAlive_;
    j["moveEasing.maxTime"]   = m.moveEasing_.maxTime;
    j["startPos"]             = m.startPos_;
    j["endPos"]               = m.endPos_;
    j["Direction"]            = static_cast<int32_t>(m.arrowDirection_);
    j["moveEasing.backRatio"] = m.moveEasing_.backRatio;
}

void from_json(const nlohmann::json& j, TutorialArrowStatus& m) {
    j.at("isAlive").get_to(m.isAlive_);
    j.at("moveEasing.maxTime").get_to(m.moveEasing_.maxTime);
    j.at("startPos").get_to(m.startPos_);
    j.at("endPos").get_to(m.endPos_);
    int32_t direction = 0;
    j.at("Direction").get_to(direction);
    m.arrowDirection_ = static_cast<ArrowDirection>(direction);
    if (auto it = j.find("moveEasing.backRatio"); it != j.end()) {
        j.at("moveEasing.backRatio").get_to(m.moveEasing_.backRatio);
    }
}

void TutorialArrowStatus::Finalize() {
    // 必要ならリソース解放などを記述
}

void TutorialArrowStatus::ArrowMoveEasing(const float& time) {
    moveEasing_.time += time;

    position_ = Back::InCircZero(startPos_, endPos_, moveEasing_.time, moveEasing_.maxTime, moveEasing_.backRatio);

    if (moveEasing_.time < moveEasing_.maxTime) {
        return;
    }

    position_        = startPos_;
    moveEasing_.time = 0.0f;
}

void TutorialArrowStatus::Reset() {
    moveEasing_.time = 0.0f;
    position_        = startPos_;
}
