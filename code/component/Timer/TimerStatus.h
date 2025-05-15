#pragma once

#include "component/IComponent.h"
#include <cstdint>
#include <Entity.h>
#include <Vector3.h>

class TimerStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const TimerStatus& _timerStatus);
    friend void from_json(const nlohmann::json& _json, TimerStatus& _timerStatus);

private: // variables
    bool isAlive_ = true;

    float currentTimer_;
    float pulusTime_;
    float minusTime_;
    float minusTimeNormal_;
    float currentPulusTime_;
    float promiseTime_;
    bool isChanging_ = false;
    int32_t previousSecond_ = 0;

public:
    TimerStatus() {}
    virtual ~TimerStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

    void DecrementTimer();
    void CurrentTimeUpdate(const int32_t& time);
    bool IsChangeSecond();

    void MinusTimer(const float& timer);
    void TimerIncrement(const float& timer);

public: // accsessor
    /// getter
    float GetCurrentTimer() const { return currentTimer_; }
    float GetPromiseTime() const { return promiseTime_; }
    float GetPulusTime() const { return pulusTime_; }
    float GetMinusTime() const { return minusTime_; }
    float GetCurrentPulusTime() const { return currentPulusTime_; }
    float GetMinusTimeNormal() const { return minusTimeNormal_; }

    /// setter
    void SetCurrentTimer(float currentTimer) { currentTimer_ = currentTimer; }
  
};
