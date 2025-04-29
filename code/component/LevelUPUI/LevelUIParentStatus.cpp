#include "LevelUIParentStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void LevelUIParentStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    currentLevel_ = 0;
}

bool LevelUIParentStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    ImGui::Text("moveEasing");
    isChange |= ImGui::DragFloat("moveEasing.maxTime", &moveEasing_.maxTime);
    ImGui::Text("scaleEasing");
    isChange |= ImGui::DragFloat("scaleEasing.maxTime", &scaleEasing_.maxTime);
    isChange |= ImGui::DragFloat("scaleEasing.amplitude", &scaleEasing_.amplitude);
    isChange |= ImGui::DragFloat("scaleEasing.period", &scaleEasing_.period);
    ImGui::Text("uvScrollEasing");
    isChange |= ImGui::DragFloat("uvScrollEasing.maxTime", &uvScrollEasing_.maxTime);


    /*  isChange |= ImGui::InputInt("d", &currentComboNum_);*/

    return isChange;
}

void LevelUIParentStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("moveEasing.maxTime", moveEasing_.maxTime);
    _writer.Write("scaleEasing.maxTime", scaleEasing_.maxTime);
    _writer.Write("scaleEasing.amplitude", scaleEasing_.amplitude);
    _writer.Write("scaleEasing.period", scaleEasing_.period);
    _writer.Write("uvScrollEasing.maxTime", uvScrollEasing_.maxTime);
}

void LevelUIParentStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("moveEasing.maxTime", moveEasing_.maxTime);
    _reader.Read("scaleEasing.maxTime", scaleEasing_.maxTime);
    _reader.Read("scaleEasing.amplitude", scaleEasing_.amplitude);
    _reader.Read("scaleEasing.period", scaleEasing_.period);
    _reader.Read("uvScrollEasing.maxTime", uvScrollEasing_.maxTime);
}

void LevelUIParentStatus::Finalize() {}
