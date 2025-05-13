#include "ResultUIScoreStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void ResultUIScoreStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ResultUIScoreStatus::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    // ComboDigit（桁数）のUI選択
    static const char* digitLabels[] = {"ONE", "TWO", "THREE", "FLOR", "FIVE"};
    int currentIndex                 = static_cast<int>(uiDigit_);
    if (ImGui::Combo("LevelUI Digit", &currentIndex, digitLabels, static_cast<int>(ResultUIDigit::COUNT))) {
        auto command = std::make_unique<SetterCommand<ResultUIScoreStatus::ResultUIDigit>>(&uiDigit_, static_cast<ResultUIDigit>(currentIndex));
        EditorGroup::getInstance()->pushCommand(std::move(command));
    }

    isChange |= DragGuiVectorCommand<3,float>("offsetPos", offsetPos_);
    isChange |= DragGuiVectorCommand("textureSize", textureSize_);

    return isChange;
}
void ResultUIScoreStatus::Finalize() {}

int32_t ResultUIScoreStatus::GetValueForDigit(const int32_t& value) {

    switch (uiDigit_) {
    case ResultUIDigit::ONE:
        // 小数点以下第1位を取得
        valueForDigit_ = static_cast<int32_t>(value) % 10;
        break;

    case ResultUIDigit::TWO:
        // 整数部の第2位を取得
        valueForDigit_ = static_cast<int32_t>(value) / 10 % 10;
        break;

    case ResultUIDigit::THREE:
        // 整数部の第2位を取得
        valueForDigit_ = static_cast<int32_t>(value) / 100 % 10;
        break;

    case ResultUIDigit::FOUR:
        // 整数部の第2位を取得
        valueForDigit_ = static_cast<int32_t>(value) / 1000 % 10;
        break;

    case ResultUIDigit::FIVE:
        // 整数部の第2位を取得
        valueForDigit_ = static_cast<int32_t>(value) / 10000 % 10;
        break;

    default:
        return 0;
    }
    return valueForDigit_;
}

void to_json(nlohmann::json& _json, const ResultUIScoreStatus& _component) {
    _json["isAlive"]      = _component.isAlive_;
    _json["levelUIDigit"] = static_cast<int32_t>(_component.uiDigit_);
    _json["offsetPos"]    = _component.offsetPos_;
    _json["textureSize"]  = _component.textureSize_;
}
void from_json(const nlohmann::json& _json, ResultUIScoreStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    int32_t digit = 0;
    _json.at("levelUIDigit").get_to(digit);
    _component.uiDigit_ = static_cast<ResultUIScoreStatus::ResultUIDigit>(digit);
    _json.at("offsetPos").get_to(_component.offsetPos_);
    _json.at("textureSize").get_to(_component.textureSize_);
}
