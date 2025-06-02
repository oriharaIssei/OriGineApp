#include "GameEndUIStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void GameEndUIStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool GameEndUIStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG
    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiVectorCommand("maxScale", maxScale_, 0.01f);

    ImGui::Text("alphaEasing");
    isChange |= DragGuiCommand("alphaEasing.maxTime", alphaEasing_.maxTime, 0.01f);
    ImGui::Text("scaleEasing");
    isChange |= DragGuiCommand("apeerEasing.maxTime", apeerEasing_.maxTime, 0.01f);

    ImGui::Text("waitTime");
    isChange |= DragGuiCommand("waitTimeAfterApearEase_", waitTimeAfterApearEase_, 0.01f);

#endif // _DEBUG

    return isChange;
}

void GameEndUIStatus::Finalize() {}

void GameEndUIStatus::ApeerUIAnimation(const float& time) {
    apeerEasing_.time += time;
    baseScale_ = EaseOutQuad(Vec2f(0.0f, 0.0f), maxScale_, apeerEasing_.time, apeerEasing_.maxTime);

    if (apeerEasing_.time < apeerEasing_.maxTime) {
        return;
    }

    baseScale_        = maxScale_;
    apeerEasing_.time = apeerEasing_.maxTime;
}

void GameEndUIStatus::AlphaEaseAnimation(const float& time) {
    alphaEasing_.time += time;
    alpha_ = EaseInSine(0.0f, 1.0f, alphaEasing_.time, alphaEasing_.maxTime);

    if (alphaEasing_.time < alphaEasing_.maxTime) {
        return;
    }

    alpha_            = 1.0f;
    alphaEasing_.time = alphaEasing_.maxTime;
}

void GameEndUIStatus::CheckAbleNextStep() {
    if (alphaEasing_.time < alphaEasing_.maxTime) {
        return;
    }

    if (apeerEasing_.time < apeerEasing_.maxTime) {
        return;
    }

    curerntStep_ = GameEndUIStep::WAIT;
}

void GameEndUIStatus::Reset() {
    baseScale_        = Vec2f(0.0f, 0.0f);
    alpha_            = 0.0f;
    apeerEasing_.time = 0.0f;
    apeerEasing_.time = 0.0f;
}

void to_json(nlohmann::json& _json, const GameEndUIStatus& _component) {
    _json["isAlive"]                = _component.isAlive_;
    _json["zoomEasing_.maxTime"]    = _component.apeerEasing_.maxTime;
    _json["waitTimeAfterApearEase"] = _component.waitTimeAfterApearEase_;
    _json["easeScale"]              = _component.maxScale_;
    _json["alphaEasingmaxTime"]     = _component.alphaEasing_.maxTime;
}

void from_json(const nlohmann::json& _json, GameEndUIStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    _json.at("zoomEasing_.maxTime").get_to(_component.apeerEasing_.maxTime);
    _json.at("waitTimeAfterApearEase").get_to(_component.waitTimeAfterApearEase_);
    _json.at("easeScale").get_to(_component.maxScale_);
    _json.at("alphaEasingmaxTime").get_to(_component.alphaEasing_.maxTime);
}
