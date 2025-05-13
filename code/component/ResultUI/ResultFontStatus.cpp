#include "ResultFontStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "component/ResultUI/ResultUIParentStatus.h"
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void ResultFontStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ResultFontStatus::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiVectorCommand<3,float>("initPos", initPos_, 0.01f);
    isChange |= DragGuiVectorCommand<3,float>("easePos", easePos_, 0.01f);
    isChange |= DragGuiVectorCommand("easeScale", easeScale_, 0.01f);

    ImGui::Text("moveEasing");
    isChange |= DragGuiCommand("moveEasing.maxTime", moveEasing_.maxTime, 0.01f);
    ImGui::Text("scaleEasing");
    isChange |= DragGuiCommand("zoomEasing_.maxTime", zoomEasing_.maxTime, 0.01f);

    ImGui::Text("waitTime");
    isChange |= DragGuiCommand("reverseWaitTime_", moveWaitTime_, 0.01f);
    isChange |= DragGuiCommand("firstWaitTime_", firstWaitTime_, 0.01f);

    return isChange;
}

void ResultFontStatus::Finalize() {}

void ResultFontStatus::ZoomAnimation(const float& time) {
    zoomEasing_.time += time;
    baseScale_ = EaseOutBack(Vec2f(0.0f, 0.0f), easeScale_, zoomEasing_.time, zoomEasing_.maxTime);

    if (zoomEasing_.time < zoomEasing_.maxTime) {
        return;
    }

    baseScale_       = easeScale_;
    zoomEasing_.time = zoomEasing_.maxTime;
    curerntStep_     = ResultFontStep::MOVEWAIT;
}

void ResultFontStatus::MoveAnimation(const float& time, ResultUIParentStatus& parentStatus) {

    moveEasing_.time += time;

    basePos_   = EaseOutBack(initPos_, easePos_, moveEasing_.time, moveEasing_.maxTime);
    baseScale_ = EaseInCirc(easeScale_, Vec2f(1.0f, 1.0f), moveEasing_.time, moveEasing_.maxTime);

    if (moveEasing_.time >= moveEasing_.maxTime - 0.6f) {
        parentStatus.SetIsAnimation(true);
    }

    if (moveEasing_.time < moveEasing_.maxTime) {
        return;
    }

    moveEasing_.time = moveEasing_.maxTime;
    basePos_         = easePos_;
    baseScale_       = Vec2f(1.0f, 1.0f);
    curerntStep_     = ResultFontStep::END;
}

void ResultFontStatus::Reset() {
    moveEasing_.time = 0.0f;
    zoomEasing_.time = 0.0f;
    basePos_         = initPos_;
}

void to_json(nlohmann::json& _json, const ResultFontStatus& _component) {
    _json["isAlive"]             = _component.isAlive_;
    _json["moveEasing.maxTime"]  = _component.moveEasing_.maxTime;
    _json["easePos"]             = _component.easePos_;
    _json["zoomEasing_.maxTime"] = _component.zoomEasing_.maxTime;
    _json["scoreWaitTime"]       = _component.moveWaitTime_;
    _json["initPos"]             = _component.initPos_;
    _json["zoomValue"]           = _component.zoomValue_;
    _json["reverseWaitTime"]     = _component.moveWaitTime_;
    _json["firstWaitTime"]       = _component.firstWaitTime_;
    _json["easeScale"]           = _component.easeScale_;
}

void from_json(const nlohmann::json& _json, ResultFontStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    _json.at("moveEasing.maxTime").get_to(_component.moveEasing_.maxTime);
    _json.at("easePos").get_to(_component.easePos_);
    _json.at("zoomEasing_.maxTime").get_to(_component.zoomEasing_.maxTime);
    _json.at("scoreWaitTime").get_to(_component.moveWaitTime_);
    _json.at("initPos").get_to(_component.initPos_);
    _json.at("zoomValue").get_to(_component.zoomValue_);
    _json.at("reverseWaitTime").get_to(_component.moveWaitTime_);
    _json.at("firstWaitTime").get_to(_component.firstWaitTime_);
    _json.at("easeScale").get_to(_component.easeScale_);
}
