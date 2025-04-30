#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class LevelUIStatus;
class LevelUIAdaptSystem
    : public ISystem {
private:
    LevelUIStatus* levelUIStatus_;

public:
    LevelUIAdaptSystem();
    ~LevelUIAdaptSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
