#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class LevelUIParentStatus;
class LevelUIAnimationSystem
    : public ISystem {
private:
    LevelUIParentStatus* levelUIParentStatus_;

public:
    LevelUIAnimationSystem();
    ~LevelUIAnimationSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
