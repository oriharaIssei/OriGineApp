#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class ResultFontSystem
    : public ISystem {
private:
   
    float time_;
   
public:
    ResultFontSystem();
    ~ResultFontSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
