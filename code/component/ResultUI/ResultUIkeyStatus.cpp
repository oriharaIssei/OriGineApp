#include "ResultUIKeyStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void to_json(nlohmann::json& _json, const ResultUIkeyStatus& _component) {
    _json["isAlive"]             = _component.isAlive_;
    _json["moveEasing.maxTime"]  = _component.moveEasing_.maxTime;
    _json["alphaEasing.maxTime"] = _component.alphaEasing_.maxTime;
    _json["easePos"]             = _component.easePos_;
    _json["initPos"]             = _component.initPos_;
}
void from_json(const nlohmann::json& _json, ResultUIkeyStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    _json.at("moveEasing.maxTime").get_to(_component.moveEasing_.maxTime);
    _json.at("alphaEasing.maxTime").get_to(_component.alphaEasing_.maxTime);
    _json.at("easePos").get_to(_component.easePos_);
    _json.at("initPos").get_to(_component.initPos_);
}

void ResultUIkeyStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ResultUIkeyStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat3("initPos", initPos_.v);
    isChange |= ImGui::DragFloat3("easePos", easePos_.v);
    ImGui::Text("moveEasing");
    isChange |= ImGui::DragFloat("moveEasing.maxTime", &moveEasing_.maxTime, 0.01f);

    ImGui::Text("AlphaEasing");
    isChange |= ImGui::DragFloat("alphaEasing.maxTime", &alphaEasing_.maxTime, 0.01f);

    return isChange;
}

void ResultUIkeyStatus::Finalize() {}

void ResultUIkeyStatus::MoveAnimation(const float& time) {

    moveEasing_.time += time;

    basePos_ = EaseInCubic(initPos_, easePos_, moveEasing_.time, moveEasing_.maxTime);
    /* scoreScale_ = EaseInCubic(scoreInitScale_, scoreEaseScale_, moveEasing_.time, moveEasing_.maxTime);*/

    if (moveEasing_.time < moveEasing_.maxTime) {
        return;
    }

    moveEasing_.time = moveEasing_.maxTime;
    basePos_         = easePos_;
    /* scoreScale_      = scoreEaseScale_;*/
    curerntStep_ = KeyResultStep::END;
}

void ResultUIkeyStatus::AlphaAnimation(const float& time) {
    alphaEasing_.time += time;

    // 現在のUVを補間計算
    alpha_ = EaseInCirc(0.0f, 1.0f, alphaEasing_.time, alphaEasing_.maxTime);

    // UVスクロール完了時の後処理
    if (alphaEasing_.time < alphaEasing_.maxTime) {
        return;
    }

    alphaEasing_.time = alphaEasing_.maxTime;
    alpha_            = 1.0f;
}

void ResultUIkeyStatus::Reset() {
    moveEasing_.time  = 0.0f;
    alphaEasing_.time = 0.0f;
    basePos_          = initPos_;
    alpha_            = 0.0f;
}
