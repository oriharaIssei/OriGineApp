#include "ResultUIScoreStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void ResultUIScoreStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ResultUIScoreStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    // ComboDigit（桁数）のUI選択
    static const char* digitLabels[] = {"ONE", "TWO", "THREE", "FLOR", "FIVE"};
    int currentIndex                 = static_cast<int>(uiDigit_);
    if (ImGui::Combo("LevelUI Digit", &currentIndex, digitLabels, static_cast<int>(ResultUIDigit::COUNT))) {
        uiDigit_ = static_cast<ResultUIDigit>(currentIndex);
    }

    isChange |= ImGui::DragFloat3("offsetPos", offsetPos_.v);

    return isChange;
}

void ResultUIScoreStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("levelUIDigit", static_cast<int32_t>(uiDigit_));
    _writer.Write<3, float>("offsetPos", offsetPos_);
}

void ResultUIScoreStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    int32_t digit = 0;
    _reader.Read("levelUIDigit", digit);
    uiDigit_ = static_cast<ResultUIDigit>(digit);

    _reader.Read<3, float>("offsetPos", offsetPos_);
}

void ResultUIScoreStatus::Finalize() {}

int32_t ResultUIScoreStatus::GetValueForDigit(const int32_t& value) {

    int32_t intValue = 0;
    switch (uiDigit_) {
    case ResultUIDigit::ONE: {
        // 小数点以下第1位を取得
         intValue = static_cast<int32_t>(value) % 10;
        return intValue;
    }
    case ResultUIDigit::TWO: {
        // 整数部の第2位を取得
         intValue = static_cast<int32_t>(value) / 10 % 10;
        return intValue;
    }
    case ResultUIDigit::THREE: {
        // 整数部の第2位を取得
         intValue = static_cast<int32_t>(value) / 100 % 10;
        return intValue;
    }
    case ResultUIDigit::FOUR: {
        // 整数部の第2位を取得
         intValue = static_cast<int32_t>(value) / 1000 % 10;
        return intValue;
    }
    case ResultUIDigit::FIVE: {
        // 整数部の第2位を取得
         intValue = static_cast<int32_t>(value) / 10000 % 10;
        return intValue;
    }
    default:
        return 0;
    }
    return valueForDigit_;
}
