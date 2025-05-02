#include "TimerUIStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void TimerUIStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool TimerUIStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    // ComboDigit（桁数）のUI選択
    static const char* digitLabels[] = {"ONE", "TWO", "DecimalONE", "DecimalTWO"};
    int currentIndex                 = static_cast<int>(digit_);
    if (ImGui::Combo("Timer Digit", &currentIndex, digitLabels, static_cast<int>(TimeDigit::COUNT))) {
        digit_ = static_cast<TimeDigit>(currentIndex);
    }

    return isChange;
}

void TimerUIStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("timerDigit", static_cast<int32_t>(digit_));
}

void TimerUIStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    int32_t digit = 0;
    _reader.Read("timerDigit", digit);
    digit_ = static_cast<TimeDigit>(digit);
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
