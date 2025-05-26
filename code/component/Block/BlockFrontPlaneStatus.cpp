#include "BlockFrontPlaneStatus.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void to_json(nlohmann::json& _json, const BlockFrontPlaneStatus& _component) {
    _json["isAlive"]               = _component.isAlive_;
    _json["rotateSpeed"]           = _component.rotateSpeed_;
    _json["startScale"]            = _component.startScale_;
    _json["closeEasing.maxTime"]   = _component.closeEasing_.maxTime;
    _json["closeEasing.amplitude"] = _component.closeEasing_.amplitude;
    _json["closeEasing.period"]    = _component.closeEasing_.period;
    _json["closeEasing.backRatio"] = _component.closeEasing_.backRatio;
}

void from_json(const nlohmann::json& _json, BlockFrontPlaneStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    _json.at("rotateSpeed").get_to(_component.rotateSpeed_);
    _json.at("startScale").get_to(_component.startScale_);
    _json.at("closeEasing.maxTime").get_to(_component.closeEasing_.maxTime);
    _json.at("closeEasing.amplitude").get_to(_component.closeEasing_.amplitude);
    _json.at("closeEasing.period").get_to(_component.closeEasing_.period);
    _json.at("closeEasing.backRatio").get_to(_component.closeEasing_.backRatio);
}

void BlockFrontPlaneStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    // 初期化処理が必要ならここに記述
}

bool BlockFrontPlaneStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiCommand("rotateSpeed", rotateSpeed_, 0.01f);
    isChange |= DragGuiVectorCommand("startScale", startScale_);

    ImGui::Text("closeEasing");
    isChange |= DragGuiCommand("closeEasing.maxTime", closeEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("closeEasing.amplitude", closeEasing_.amplitude, 0.01f);
    isChange |= DragGuiCommand("closeEasing.period", closeEasing_.period, 0.01f);
    isChange |= DragGuiCommand("closeEasing.backRatio", closeEasing_.backRatio, 0.01f);

#endif // _DEBUG

    return isChange;
}

void BlockFrontPlaneStatus::Finalize() {}


void BlockFrontPlaneStatus::CloseEasing(const float& deltaTime) {
    closeEasing_.time += deltaTime;

    /// 　ease
    scale_ = EaseOutBack(startScale_, Vec3f(0.0f, 0.0f, 0.0f), closeEasing_.time, closeEasing_.maxTime);

    // end
    if (closeEasing_.time < closeEasing_.maxTime) {
        return;
    }

    // adapt
    closeEasing_.time = closeEasing_.maxTime;
    scale_            = Vec3f(0.0f, 0.0f, 0.0f);
    isDeath_          = true;
    planestep_        = PlaneStep::END;
}

void BlockFrontPlaneStatus::Reset() {
    closeEasing_.time = 0.0f;
    scale_            = startScale_;
}
