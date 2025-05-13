#include "ScoreUIStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void ScoreUIStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ScoreUIStatus::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", &isAlive_);

    ImGui::Spacing();

    // ComboDigit（桁数）のUI選択
    static const char* digitLabels[] = {
        "ONE",
        "TWO",
        "THREE",
        "FOUR",
        "FIVE",
        "SIX",
        "SEVEN"};
    int currentIndex = static_cast<int>(digit_);
    if (ImGui::Combo("Timer Digit", &currentIndex, digitLabels, static_cast<int>(TimeDigit::COUNT))) {
        digit_ = static_cast<TimeDigit>(currentIndex);
    }

    return isChange;
}

void ScoreUIStatus::Finalize() {}

int32_t ScoreUIStatus::GetValueForDigit(const float& value) {
    switch (digit_) {
    case TimeDigit::ONE: {
        // 小数点以下第1位を取得
        int32_t intValue = static_cast<int32_t>(value) % 10;
        return intValue;
    }
    case TimeDigit::TWO: {
        // 整数部の第2位を取得
        int32_t intValue = static_cast<int32_t>(value) / 10 % 10;
        return intValue;
    }
    case TimeDigit::THREE: {
        // 整数部の第2位を取得
        int32_t intValue = static_cast<int32_t>(value) / 100 % 10;
        return intValue;
    }
    case TimeDigit::FOUR: {
        // 整数部の第2位を取得
        int32_t intValue = static_cast<int32_t>(value) / 1000 % 10;
        return intValue;
    }
    case TimeDigit::FIVE: {
        // 整数部の第2位を取得
        int32_t intValue = static_cast<int32_t>(value) / 10000 % 10;
        return intValue;
    }
    case TimeDigit::SIX: {
        // 整数部の第2位を取得
        int32_t intValue = static_cast<int32_t>(value) / 100000 % 10;
        return intValue;
    }
    case TimeDigit::SEVEN: {
        // 整数部の第2位を取得
        int32_t intValue = static_cast<int32_t>(value) / 1000000 % 10;
        return intValue;
    }

    default:
        return 0;
    }
}

void to_json(nlohmann::json& _json, const ScoreUIStatus& _component) {
    _json["isAlive"]      = _component.isAlive_;
    _json["levelUIDigit"] = static_cast<int32_t>(_component.digit_);
}
void from_json(const nlohmann::json& _json, ScoreUIStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    int32_t digit = 0;
    _json.at("levelUIDigit").get_to(digit);
    _component.digit_ = static_cast<ScoreUIStatus::TimeDigit>(digit);
}
