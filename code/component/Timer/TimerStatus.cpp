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

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiCommand("startTimer", currentTimer_, 0.01f);
    isChange |= DragGuiCommand("pulusTime", pulusTime_, 0.01f);
    isChange |= DragGuiCommand("minusTime", minusTime_, 0.01f);
    isChange |= DragGuiCommand("minusTimeNormal", minusTimeNormal_, 0.01f);
    isChange |= DragGuiCommand("promiseTime", promiseTime_, 0.01f);

    ImGui::Text("NotChange");

    isChange |= DragGuiCommand("currentPulusTime", currentPulusTime_, 0.01f);

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

void TimerStatus::MinusTimer(const float& timer) {
    if (currentTimer_ < promiseTime_) {
        return;
    }

    float preTime = currentTimer_ - timer;
    if (preTime < promiseTime_) {
        currentTimer_ = promiseTime_;
        return;
    }

        // そのまま減算
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
    _json["promiseTime"]      = _timerStatus.promiseTime_;
}

void from_json(const nlohmann::json& _json, TimerStatus& _timerStatus) {
    _json.at("isAlive").get_to(_timerStatus.isAlive_);
    _json.at("pulusTime").get_to(_timerStatus.pulusTime_);
    _json.at("minusTime").get_to(_timerStatus.minusTime_);
    _json.at("startTimer").get_to(_timerStatus.currentTimer_);
    _json.at("currentPulusTime").get_to(_timerStatus.currentPulusTime_);
    _json.at("minusTimeNormal").get_to(_timerStatus.minusTimeNormal_);
    if (auto it = _json.find("promiseTime"); it != _json.end()) {
        _json.at("promiseTime").get_to(_timerStatus.promiseTime_);
    }
}
