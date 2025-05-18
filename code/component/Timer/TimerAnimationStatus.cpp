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

#ifdef _DEBUG
    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    ImGui::Text("timer");
    isChange |= DragGuiVectorCommand("maxScale", maxScale_, 0.01f);
    isChange |= DragGuiVectorCommand("initScale", initScale_, 0.01f);
    isChange |= DragGuiVectorCommand("textureSize", textureSize_, 0.01f);
    isChange |= DragGuiVectorCommand("panicColor", panicColor_, 0.01f);

    ImGui::Text("colorChangeEasing");
    isChange |= DragGuiCommand("colorChangeEasing.maxTime", colorChangeEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("colorChangeEasing.backRatio", colorChangeEasing_.backRatio, 0.01f);
    ImGui::Text("scaleEasing");
    isChange |= DragGuiCommand("scalingEasing.maxTime", scalingEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("scalingEasing.backRatio", scalingEasing_.backRatio, 0.01f);

    ImGui::Text("waitTime");
    isChange |= DragGuiCommand("waitTimeAfterScaing", waitTimeAfterScaing_, 0.01f);
#endif // _DEBUG

    ImGui::SeparatorText("Back");

    ImGui::Text("backtimer");
    isChange |= DragGuiVectorCommand("backmaxScale", backMaxScale_, 0.01f);
    isChange |= DragGuiVectorCommand("backinitScale", backInitScale_, 0.01f);

    ImGui::Text("backAlpha");
    isChange |= DragGuiCommand("backStartAlpha", backStartAlpha_, 0.01f);
    isChange |= DragGuiCommand("backEndAlpha", backEndAlpha_, 0.01f);

    ImGui::Text("backApearEase");
    isChange |= DragGuiCommand("backApearEase.maxTime", backApearEase_.maxTime, 0.01f);
    ImGui::Text("backAlphaEase");
    isChange |= DragGuiCommand("backAlphaEase.maxTime", backAlphaEase_.maxTime, 0.01f);
    isChange |= DragGuiCommand("backAlphaEase.backRatio", backAlphaEase_.backRatio, 0.01f);

    return isChange;
}

void TimerAnimationStatus::Finalize() {}

void TimerAnimationStatus::ScalingAnimation(const float& time) {

    color_ = panicColor_;

    scalingEasing_.time += time;
    baseScale_ = Back::InCubicZero(initScale_, maxScale_, scalingEasing_.time, scalingEasing_.maxTime, scalingEasing_.backRatio);

    if (scalingEasing_.time < scalingEasing_.maxTime) {
        return;
    }

    baseScale_          = initScale_;
    scalingEasing_.time = scalingEasing_.maxTime;
    curerntStep_        = TimerAnimationStep::END;
}

void TimerAnimationStatus::BackApearScaling(const float& time) {

    backApearEase_.time += time;
    backBaseScale_ = EaseOutQuad(backInitScale_, backMaxScale_, backApearEase_.time, backApearEase_.maxTime);

    if (backApearEase_.time < backApearEase_.maxTime) {
        return;
    }

    backBaseScale_      = backMaxScale_;
    backApearEase_.time = backApearEase_.maxTime;
}

void TimerAnimationStatus::BackAlphaEasing(const float& time) {

    backAlphaEase_.time += time;
    backAlpha_ = Back::InCubicZero(backStartAlpha_, backEndAlpha_, backAlphaEase_.time, backAlphaEase_.maxTime, backAlphaEase_.backRatio);

    if (backAlphaEase_.time < backAlphaEase_.maxTime) {
        return;
    }

    backAlpha_          = backEndAlpha_;
    backAlphaEase_.time = backAlphaEase_.maxTime;
}

void TimerAnimationStatus::ColorChangeEasing(const float& time) {
    color_ = panicColor_;
    time;
}

void TimerAnimationStatus::CheckAbleNextStep() {

    if (scalingEasing_.time < scalingEasing_.maxTime) {
        return;
    }

    if (backApearEase_.time < backApearEase_.maxTime) {
        return;
    }

    if (backAlphaEase_.time < backAlphaEase_.maxTime) {
        return;
    }

    curerntStep_ = TimerAnimationStep::END;
}

void TimerAnimationStatus::Reset() {
    baseScale_              = initScale_;
    color_                  = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    colorChangeEasing_.time = 0.0f;
    scalingEasing_.time     = 0.0f;

    backApearEase_.time = 0.0f;
    backAlphaEase_.time = 0.0f;
    backAlpha_          = backStartAlpha_;
    backBaseScale_      = backInitScale_;
}

void to_json(nlohmann::json& _json, const TimerAnimationStatus& _component) {
    _json["isAlive"]                      = _component.isAlive_;
    _json["zoomEasing_.maxTime"]          = _component.scalingEasing_.maxTime;
    _json["waitTimeAfterScaing"]          = _component.waitTimeAfterScaing_;
    _json["maxScale"]                     = _component.maxScale_;
    _json["initScale"]                    = _component.initScale_;
    _json["alphaEasingmaxTime"]           = _component.colorChangeEasing_.maxTime;
    _json["textureSize"]                  = _component.textureSize_;
    _json["panicColor"]                   = _component.panicColor_;
    _json["colorChangeEasing_.backRatio"] = _component.colorChangeEasing_.backRatio;
    _json["scalingEasing_.backRatio"]     = _component.scalingEasing_.backRatio;

    _json["backMaxScale"]             = _component.backMaxScale_;
    _json["backInitScale"]            = _component.backInitScale_;
    _json["backStartAlpha"]           = _component.backStartAlpha_;
    _json["backEndAlpha"]             = _component.backEndAlpha_;
    _json["backApearEase_.maxTime"]   = _component.backApearEase_.maxTime;
    _json["backAlphaEase_.maxTime"]   = _component.backAlphaEase_.maxTime;
    _json["backAlphaEase_.backRatio"] = _component.backAlphaEase_.backRatio;
}

void from_json(const nlohmann::json& _json, TimerAnimationStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    _json.at("zoomEasing_.maxTime").get_to(_component.scalingEasing_.maxTime);
    _json.at("waitTimeAfterScaing").get_to(_component.waitTimeAfterScaing_);
    _json.at("maxScale").get_to(_component.maxScale_);
    _json.at("initScale").get_to(_component.initScale_);
    _json.at("alphaEasingmaxTime").get_to(_component.colorChangeEasing_.maxTime);
    _json.at("textureSize").get_to(_component.textureSize_);
    _json.at("panicColor").get_to(_component.panicColor_);
    _json.at("colorChangeEasing_.backRatio").get_to(_component.colorChangeEasing_.backRatio);
    _json.at("scalingEasing_.backRatio").get_to(_component.scalingEasing_.backRatio);

    if (auto it = _json.find("backMaxScale"); it != _json.end()) {
        _json.at("backMaxScale").get_to(_component.backMaxScale_);
    }

    if (auto it = _json.find("backInitScale"); it != _json.end()) {
        _json.at("backInitScale").get_to(_component.backInitScale_);
    }

    if (auto it = _json.find("backStartAlpha"); it != _json.end()) {
        _json.at("backStartAlpha").get_to(_component.backStartAlpha_);
    }

    if (auto it = _json.find("backEndAlpha"); it != _json.end()) {
        _json.at("backEndAlpha").get_to(_component.backEndAlpha_);
    }

    if (auto it = _json.find("backApearEase_.maxTime"); it != _json.end()) {
        _json.at("backApearEase_.maxTime").get_to(_component.backApearEase_.maxTime);
    }

    if (auto it = _json.find("backAlphaEase_.maxTime"); it != _json.end()) {
        _json.at("backAlphaEase_.maxTime").get_to(_component.backAlphaEase_.maxTime);
    }

    if (auto it = _json.find("backAlphaEase_.backRatio"); it != _json.end()) {
        _json.at("backAlphaEase_.backRatio").get_to(_component.backAlphaEase_.backRatio);
    }
}
