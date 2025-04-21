#pragma once

#include "component/IComponent.h"
#include <array>
#include <component/transform/Transform.h>
#include <cstdint>
#include <Entity.h>
#include <Vector3.h>

class TimerStatus
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    float currentTimer_;
    float pulusTime_;
    float minusTime_;
    float minusTimeNormal_;
    float currentPulusTime_;
    bool isChanging_ = false;

 

public:
    TimerStatus() {}
    virtual ~TimerStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

     void DecrementTimer();
    void CurrentTimeUpdate(const int32_t& time);
     void PlusTimeUpdate();

     void TimerDecrement(const float& timer);
     void TimerIncrement(const float& timer);

public: // accsessor
    /// getter
    float GetCurrentTimer() const { return currentTimer_; }
    float GetPulusTime() const { return pulusTime_; }
    float GetMinusTime() const { return minusTime_; }
    float GetCurrentPulusTime() const { return currentPulusTime_; }
    float GetMinusTimeNormal() const { return minusTimeNormal_; }
   
    /// setter
    void SetCurrentTimer(float currentTimer) { currentTimer_ = currentTimer; }
   /* void SetDecrementTimer(const float& time) { decrementTimer_ = time; }
    void SetIncrementTimer(const float& time) { incrementTimer_ = time; }*/
   
};
