#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"


class TutorialMenuSystem
    : public ISystem {
private:
 
    float cutNum_;

public:
    TutorialMenuSystem();
    ~TutorialMenuSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
