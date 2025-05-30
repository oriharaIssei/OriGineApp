#include "DeleteEntityStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void DeleteEntityStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool DeleteEntityStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiCommand("deleteTime", deleteTime_, 0.01f);
  
#endif

    return isChange;
}

void DeleteEntityStatus::Finalize() {}


void to_json(nlohmann::json& _json, const DeleteEntityStatus& _timerStatus) {
    _json["deleteTime"] = _timerStatus.deleteTime_;
 
}

void from_json(const nlohmann::json& _json, DeleteEntityStatus& _timerStatus) {
    _json.at("deleteTime").get_to(_timerStatus.deleteTime_);
 
}

void DeleteEntityStatus::DecrementTimer(const float& time) {
    deleteTime_ -= time;
 }
