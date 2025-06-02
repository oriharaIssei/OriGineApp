#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"


class TutorialArrowSystem
    : public ISystem {
private:
 
   
public:
    TutorialArrowSystem();
    ~TutorialArrowSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
