#include "FuseStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void FuseStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool FuseStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    ImGui::Text("NotChange");

#endif

    return isChange;
}

void FuseStatus::Finalize() {}

void to_json(nlohmann::json& _json, const FuseStatus& _timerStatus) {
    _json["isAlive"] = _timerStatus.isAlive_;
}

void from_json(const nlohmann::json& _json, FuseStatus& _timerStatus) {
    _json.at("isAlive").get_to(_timerStatus.isAlive_);
}

void FuseStatus::ScaleEasing(const float& deltaTime) {
    easing_.time += deltaTime;

    scale_ = EaseOutCirc(StartScale_, maxScale_, easing_.time, easing_.maxTime);

    if (easing_.time < easing_.maxTime) {
        return;
    }

    scale_       = maxScale_;
    easing_.time = easing_.maxTime;
}

void FuseStatus::Reset() {
    easing_.time = 0.0f;
    easing_.maxTime = 0.35f;
    scale_       = StartScale_;
}
