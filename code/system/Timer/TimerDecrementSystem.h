#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class TimerStatus;
class TimerDecrementSystem
    : public ISystem {
private:
    TimerStatus* timerStauts_;

public:
    TimerDecrementSystem();
    ~TimerDecrementSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
