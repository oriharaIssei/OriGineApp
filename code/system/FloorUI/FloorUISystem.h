#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class Input;
class FloorUISystem
    : public ISystem {
private:
   
    float time_;
    Input* input_;
   
public:
    FloorUISystem();
    ~FloorUISystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
