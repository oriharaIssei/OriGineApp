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

#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiCommand("fieldRightMax", fieldRightMax_, 0.01f);
    isChange |= DragGuiCommand("fieldLeftMax", fieldLeftMax_, 0.01f);

#endif

    return isChange;
}

void FieldStatus::Finalize() {}

void to_json(nlohmann::json& _json, const FieldStatus& _component) {
    _json["isAlive"]       = _component.isAlive_;
    _json["fieldRightMax"] = _component.fieldRightMax_;
    _json["fieldLeftMax"]  = _component.fieldLeftMax_;
}
void from_json(const nlohmann::json& _json, FieldStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    _json.at("fieldRightMax").get_to(_component.fieldRightMax_);
    _json.at("fieldLeftMax").get_to(_component.fieldLeftMax_);
}
