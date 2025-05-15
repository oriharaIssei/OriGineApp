#include "TimerAnimationStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void TimerAnimationStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool TimerAnimationStatus::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiVectorCommand("maxScale", maxScale_, 0.01f);
    isChange |= DragGuiVectorCommand("initScale", initScale_, 0.01f);
    isChange |= DragGuiVectorCommand("textureSize", textureSize_, 0.01f);

    ImGui::Text("colorChangeEasing");
    isChange |= DragGuiCommand("colorChangeEasing.maxTime", colorChangeEasing_.maxTime, 0.01f);
    ImGui::Text("scaleEasing");
    isChange |= DragGuiCommand("apeerEasing.maxTime", scalingEasing_.maxTime, 0.01f);

    ImGui::Text("waitTime");
    isChange |= DragGuiCommand("waitTimeAfterScaing", waitTimeAfterScaing_, 0.01f);

    return isChange;
}

void TimerAnimationStatus::Finalize() {}

void TimerAnimationStatus::ScalingAnimation(const float& time) {
    scalingEasing_.time += time;
    baseScale_ = Back::InCubicZero(initScale_, maxScale_, scalingEasing_.time, scalingEasing_.maxTime, scalingEasing_.backRatio);

    if (scalingEasing_.time < scalingEasing_.maxTime) {
        return;
    }

    baseScale_          = initScale_;
    scalingEasing_.time = scalingEasing_.maxTime;
}

void TimerAnimationStatus::CheckAbleNextStep() {
    /*if (alphaEasing_.time < alphaEasing_.maxTime) {
        return;
    }

    if (scalingEasing_.time < scalingEasing_.maxTime) {
        return;
    }

    curerntStep_ = TimerAnimationStep::END;*/
}

void TimerAnimationStatus::Reset() {

    scalingEasing_.time = 0.0f;
}

void to_json(nlohmann::json& _json, const TimerAnimationStatus& _component) {
    _json["isAlive"]             = _component.isAlive_;
    _json["zoomEasing_.maxTime"] = _component.scalingEasing_.maxTime;
    _json["waitTimeAfterScaing"] = _component.waitTimeAfterScaing_;
    _json["maxScale"]            = _component.maxScale_;
    _json["initScale"]           = _component.initScale_;
    _json["alphaEasingmaxTime"]  = _component.colorChangeEasing_.maxTime;
    _json["textureSize"]           = _component.textureSize_;
}

void from_json(const nlohmann::json& _json, TimerAnimationStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    _json.at("zoomEasing_.maxTime").get_to(_component.scalingEasing_.maxTime);
    _json.at("waitTimeAfterScaing").get_to(_component.waitTimeAfterScaing_);
    _json.at("maxScale").get_to(_component.maxScale_);
    _json.at("initScale").get_to(_component.initScale_);
    _json.at("alphaEasingmaxTime").get_to(_component.colorChangeEasing_.maxTime);
    _json.at("textureSize").get_to(_component.textureSize_);
}
