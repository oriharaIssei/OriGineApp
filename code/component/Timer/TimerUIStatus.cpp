#include "TimerUIStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void TimerUIStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool TimerUIStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG
    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    // ComboDigit（桁数）のUI選択
    static const char* digitLabels[] = {"ONE", "TWO", "DecimalONE", "DecimalTWO"};
    int currentIndex                 = static_cast<int>(digit_);
    if (ImGui::Combo("Timer Digit", &currentIndex, digitLabels, static_cast<int>(TimeDigit::COUNT))) {
        auto command = std::make_unique<SetterCommand<int32_t>>(&currentIndex, currentIndex);
        EditorGroup::getInstance()->pushCommand(std::move(command));
        isChange = true;
    }
#endif

    return isChange;
}

void TimerUIStatus::Finalize() {}

int32_t TimerUIStatus::GetValueForDigit(const float& value) {
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
    case TimeDigit::DecimalONE: {
        // 小数点以下第1位を取得
        int32_t intValue = static_cast<int32_t>(value * 10) % 10;
        return intValue;
    }
    case TimeDigit::DecimalTWO: {
        // 小数点以下第2位を取得
        int32_t intValue = static_cast<int32_t>(value * 100) % 10;
        return intValue;
    }

    default:
        return 0;
    }
}

void to_json(nlohmann::json& _json, const TimerUIStatus& _block) {
    _json["isAlive"]    = _block.isAlive_;
    _json["timerDigit"] = static_cast<int32_t>(_block.digit_);
}

void from_json(const nlohmann::json& _json, TimerUIStatus& _block) {
    _json.at("isAlive").get_to(_block.isAlive_);
    _json.at("timerDigit").get_to(_block.digit_);
}
