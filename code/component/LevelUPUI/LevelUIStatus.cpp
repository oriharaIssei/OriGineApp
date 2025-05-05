#include "LevelUIStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void LevelUIStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool LevelUIStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    // ComboDigit（桁数）のUI選択
    static const char* digitLabels[] = {"ICON", "ONE", "TWO"};
    int currentIndex                 = static_cast<int>(levelUIDigit_);
    if (ImGui::Combo("LevelUI Digit", &currentIndex, digitLabels, static_cast<int>(LevelUIDigit::COUNT))) {
        levelUIDigit_ = static_cast<LevelUIDigit>(currentIndex);
    }

    isChange |= ImGui::DragFloat3("offsetPos", offsetPos_.v);

    return isChange;
}

void LevelUIStatus::Finalize() {}

int32_t LevelUIStatus::GetValueForDigit(const int32_t& value) {
    switch (levelUIDigit_) {
    case LevelUIDigit::ONE:
        valueForDigit_ = value % 10;
        break;
    case LevelUIDigit::TWO:
        valueForDigit_ = (value / 10) % 10;
        break;
    default:
        valueForDigit_ = 0;
        break;
    }
    return valueForDigit_;
}

void to_json(nlohmann::json& j, const LevelUIStatus& l) {
    j["isAlive"]      = l.isAlive_;
    j["levelUIDigit"] = static_cast<int32_t>(l.levelUIDigit_);
    j["offsetPos"]    = l.offsetPos_;
}

void from_json(const nlohmann::json& j, LevelUIStatus& l) {
    j.at("isAlive").get_to(l.isAlive_);
    int32_t digit = 0;
    j.at("levelUIDigit").get_to(digit);
    l.levelUIDigit_ = static_cast<LevelUIDigit>(digit);
    j.at("offsetPos").get_to(l.offsetPos_);
}
