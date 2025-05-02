#include "ComboUIStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void ComboUIStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ComboUIStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    // ComboDigit（桁数）のUI選択
    static const char* digitLabels[] = {"ONE", "TWO", "THREE"};
    int currentIndex                 = static_cast<int>(timeDigit_);
    if (ImGui::Combo("Combo Digit", &currentIndex, digitLabels, static_cast<int>(TimeDigit::COUNT))) {
        timeDigit_ = static_cast<TimeDigit>(currentIndex);
    }

    return isChange;
}

void ComboUIStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("comboDigit", static_cast<int32_t>(timeDigit_));
}

void ComboUIStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    int32_t digit = 0;
    _reader.Read("comboDigit", digit);
    timeDigit_ = static_cast<TimeDigit>(digit);
}

void ComboUIStatus::Finalize() {}

int32_t ComboUIStatus::GetValueForDigit(const int32_t& value) {
    switch (timeDigit_) {
    case TimeDigit::ONE:
        valueForDigit_ = value % 10;
        break;
    case TimeDigit::TWO:
        valueForDigit_ = (value/10)%10 ;
        break;
    case TimeDigit::THREE:
        valueForDigit_ = (value / 100) % 10;
        break;
    default:
        valueForDigit_ = 0;
        break;
    }
    return valueForDigit_;
}
