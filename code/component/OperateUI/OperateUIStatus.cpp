#include "OperateUIStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void OperateUIStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool OperateUIStatus::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", &isAlive_);

    ImGui::Spacing();
    isChange = DragGuiCommand("scalingEase_.maxTime", scalingEase_.maxTime);
    isChange = DragGuiCommand("scalingEase_.period", scalingEase_.period);
    isChange = DragGuiCommand("scalingEase_.amplitude", scalingEase_.amplitude);
    isChange = DragGuiCommand("startTextureSize_", startTextureSize_);
    isChange = DragGuiCommand("maxTextureSize_", maxTextureSize_);
    isChange = DragGuiCommand("resultTextureSize_", resultTextureSize_);
    return isChange;
}

void OperateUIStatus::Finalize() {}

void OperateUIStatus::ChangeInit(const OperateMode& operateMOde) {
    scalingEase_.time = 0.0f;
    operateMode_      = operateMOde;
    isChange_         = true;
}

void OperateUIStatus::ScalingEaseUpdate(const float& t) {
    if (!isChange_) {
        return;
    }
      
        scalingEase_.time += t;

        /// スケーリングイージング
        resultTextureSize_ = (EaseAmplitudeScale(startTextureSize_, scalingEase_.time, scalingEase_.maxTime,
            scalingEase_.amplitude, scalingEase_.period));

        // 事後処理
        if (scalingEase_.time < scalingEase_.maxTime) {
            return;
        }
        scalingEase_.time = scalingEase_.maxTime;
        resultTextureSize_ = startTextureSize_;
        isChange_         = false;
        
}

void to_json(nlohmann::json& j, const OperateUIStatus& l) {
    j["isAlive"]                = l.isAlive_;
    j["scalingEase_.maxTime"]   = l.scalingEase_.maxTime;
    j["scalingEase_.period"]    = l.scalingEase_.period;
    j["scalingEase_.amplitude"] = l.scalingEase_.amplitude;
    j["startTextureSize_"]      = l.startTextureSize_;
    j["maxTextureSize_"]        = l.maxTextureSize_;
    j["resultTextureSize_"]     = l.resultTextureSize_;
}

void from_json(const nlohmann::json& j, OperateUIStatus& l) {
    j.at("isAlive").get_to(l.isAlive_);
    j.at("scalingEase_.maxTime").get_to(l.scalingEase_.maxTime);
    j.at("scalingEase_.period").get_to(l.scalingEase_.period);
    j.at("scalingEase_.amplitude").get_to(l.scalingEase_.amplitude);
    j.at("startTextureSize_").get_to(l.startTextureSize_);
    j.at("maxTextureSize_").get_to(l.maxTextureSize_);
    j.at("resultTextureSize_").get_to(l.resultTextureSize_);
}
