#include "BlockBreakEffectStatus.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void to_json(nlohmann::json& _json, const BlockBreakEffectStatus& _component) {
    _json["isAlive"]               = _component.isAlive_;
   
}

void from_json(const nlohmann::json& _json, BlockBreakEffectStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
   
}

void BlockBreakEffectStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    // 初期化処理が必要ならここに記述
}

bool BlockBreakEffectStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();


#endif // _DEBUG

    return isChange;
}

void BlockBreakEffectStatus::Finalize() {}


void BlockBreakEffectStatus::CloseEasing(const float& deltaTime) {
    deltaTime;
    //rotate_ += rotateSpeed_ * deltaTime;
    //closeEasing_.time += deltaTime;

    ///// 　ease
    //scale_ = EaseInBack(startScale_, Vec3f(0.0f, 0.0f, 0.0f), closeEasing_.time, closeEasing_.maxTime);

    //// end
    //if (closeEasing_.time < closeEasing_.maxTime) {
    //    return;
    //}

    //// adapt
    //closeEasing_.time = closeEasing_.maxTime;
    //scale_            = Vec3f(0.0f, 0.0f, 0.0f);
    //rotate_           = 0.0f;
    //rotateSpeed_      = 0.0f;
    //isDeath_          = true;
    //planestep_        = PlaneStep::END;
}

void BlockBreakEffectStatus::Reset() {
  /*  rotate_           = 0.0f;
    closeEasing_.time = 0.0f;
    scale_            = startScale_;*/
}

void BlockBreakEffectStatus::GetParameters(const BlockBreakEffectSpawner& parm){
    parm;

} 
