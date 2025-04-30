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
    static const char* digitLabels[] = { "ICON", "ONE", "TWO"};
    int currentIndex                 = static_cast<int>(levelUIDigit_);
    if (ImGui::Combo("LevelUI Digit", &currentIndex, digitLabels, static_cast<int>(LevelUIDigit::COUNT))) {
        levelUIDigit_ = static_cast<LevelUIDigit>(currentIndex);
    }

     isChange |= ImGui::DragFloat3("offsetPos", offsetPos_.v);

    return isChange;
}

void LevelUIStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("levelUIDigit", static_cast<int32_t>(levelUIDigit_));
    _writer.Write<3, float>("offsetPos", offsetPos_);
}

void LevelUIStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    int32_t digit = 0;
    _reader.Read("levelUIDigit", digit);
    levelUIDigit_ = static_cast<LevelUIDigit>(digit);

    _reader.Read<3, float>("offsetPos", offsetPos_);
}

void LevelUIStatus::Finalize() {}

int32_t LevelUIStatus::GetValueForDigit(const int32_t& value) {
    switch (levelUIDigit_) {
    case LevelUIDigit::ONE:
        valueForDigit_ = value % 10;
        break;
    case LevelUIDigit::TWO:
        valueForDigit_ = (value/10)%10 ;
        break;
    default:
        valueForDigit_ = 0;
        break;
    }
    return valueForDigit_;
}
