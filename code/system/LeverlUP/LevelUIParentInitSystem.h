#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class TimerStatus;
class LevelUIParentInitSystem
    : public ISystem {
private:
    TimerStatus* timerStauts_;

public:
    LevelUIParentInitSystem();
    ~LevelUIParentInitSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
