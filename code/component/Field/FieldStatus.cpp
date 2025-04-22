#include "FieldStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void FieldStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool FieldStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("fieldRightMax", &fieldRightMax_, 0.01f);
    isChange |= ImGui::DragFloat("fieldLeftMax", &fieldLeftMax_, 0.01f);
   
    return isChange;
}

void FieldStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("fieldRightMax", fieldRightMax_);
    _writer.Write("fieldLeftMax", fieldLeftMax_);
}

void FieldStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("fieldRightMax", fieldRightMax_);
    _reader.Read("fieldLeftMax", fieldLeftMax_);
}

void FieldStatus::Finalize() {}

