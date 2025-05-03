#include "TutorialMenuStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void TutorialMenuStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool TutorialMenuStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    // ComboDigit（桁数）のUI選択
    static const char* digitLabels[] = {"ONE", "TWO", "THREE"};
    int currentIndex                 = static_cast<int>(uiDigit_);
    if (ImGui::Combo("Pauge num", &currentIndex, digitLabels, static_cast<int>(TutorialPauge::COUNT))) {
        uiDigit_ = static_cast<TutorialPauge>(currentIndex);
    }

    isChange |= ImGui::DragFloat2("offsetPos", offsetPos_.v);
    isChange |= ImGui::DragFloat2("textureSize", textureSize_.v);

    return isChange;
}

void TutorialMenuStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("levelUIDigit", static_cast<int32_t>(uiDigit_));
    _writer.Write<2, float>("offsetPos", offsetPos_);
    _writer.Write<2, float>("textureSize", textureSize_);
}

void TutorialMenuStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    int32_t digit = 0;
    _reader.Read("levelUIDigit", digit);
    uiDigit_ = static_cast<TutorialPauge>(digit);
    _reader.Read<2, float>("offsetPos", offsetPos_);
    _reader.Read<2, float>("textureSize", textureSize_);
}

void TutorialMenuStatus::Finalize() {}


