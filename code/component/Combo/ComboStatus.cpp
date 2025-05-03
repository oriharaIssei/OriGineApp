#include "ComboStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void ComboStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    currentComboNum_ = 0;
    maxCombvoNum_    = 0;
    isUpdateCombo_   = false;
    currentTime_     = 0.0f;
}

bool ComboStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("continuationTime", &continuationTime_, 0.01f);
    /*  isChange |= ImGui::InputInt("d", &currentComboNum_);*/

    return isChange;
}

void ComboStatus::Finalize() {}

void to_json(nlohmann::json& _json, const ComboStatus& _block) {
    _json["isAlive"]          = _block.isAlive_;
    _json["currentComboNum"]  = _block.currentComboNum_;
    _json["maxCombvoNum"]     = _block.maxCombvoNum_;
    _json["continuationTime"] = _block.continuationTime_;
    _json["currentTime"]      = _block.currentTime_;
}
void from_json(const nlohmann::json& _json, ComboStatus& _block) {
    _json.at("isAlive").get_to(_block.isAlive_);
    _json.at("currentComboNum").get_to(_block.currentComboNum_);
    _json.at("maxCombvoNum").get_to(_block.maxCombvoNum_);
    _json.at("continuationTime").get_to(_block.continuationTime_);
    _json.at("currentTime").get_to(_block.currentTime_);
}
