#include "TimerStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void TimerStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool TimerStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("startTimer", &currentTimer_, 0.01f);
    isChange |= ImGui::DragFloat("pulusTime", &pulusTime_, 0.01f);
    isChange |= ImGui::DragFloat("minusTime", &minusTime_, 0.01f);
    isChange |= ImGui::DragFloat("minusTimeNormal", &minusTimeNormal_, 0.01f);

    ImGui::Text("NotChange");

    isChange |= ImGui::DragFloat("currentPulusTime", &currentPulusTime_, 0.01f);

    return isChange;
}

void TimerStatus::Finalize() {}

void TimerStatus::DecrementTimer() {
    currentTimer_ -= Engine::getInstance()->getDeltaTime();
    if (currentTimer_ < 0.0f) {
        currentTimer_ = 0.0f;
    }
}

void TimerStatus::CurrentTimeUpdate(const int32_t& comboNum) {
    currentTimer_ += pulusTime_ * float(comboNum);
}

void TimerStatus::TimerDecrement(const float& timer) {
    currentTimer_ -= timer;
}
void TimerStatus::TimerIncrement(const float& timer) {
    currentTimer_ += timer;
}

void to_json(nlohmann::json& _json, const TimerStatus& _timerStatus) {
    _json["isAlive"]          = _timerStatus.isAlive_;
    _json["pulusTime"]        = _timerStatus.pulusTime_;
    _json["minusTime"]        = _timerStatus.minusTime_;
    _json["startTimer"]       = _timerStatus.currentTimer_;
    _json["currentPulusTime"] = _timerStatus.currentPulusTime_;
    _json["minusTimeNormal"]  = _timerStatus.minusTimeNormal_;
}

void from_json(const nlohmann::json& _json, TimerStatus& _timerStatus) {
    _json.at("isAlive").get_to(_timerStatus.isAlive_);
    _json.at("pulusTime").get_to(_timerStatus.pulusTime_);
    _json.at("minusTime").get_to(_timerStatus.minusTime_);
    _json.at("startTimer").get_to(_timerStatus.currentTimer_);
    _json.at("currentPulusTime").get_to(_timerStatus.currentPulusTime_);
    _json.at("minusTimeNormal").get_to(_timerStatus.minusTimeNormal_);
}
