#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class ScoreStatus;
class ScoreIncrementSystem
    : public ISystem {
private:
    ScoreStatus* scoreStatus_;
    float time_;

public:
    ScoreIncrementSystem();
    ~ScoreIncrementSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;


};
