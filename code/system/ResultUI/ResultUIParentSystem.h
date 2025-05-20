#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class Input;
class ResultUIParentSystem
    : public ISystem {
private:
   
    float time_;
    Input* input_;
   
public:
    ResultUIParentSystem();
    ~ResultUIParentSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
