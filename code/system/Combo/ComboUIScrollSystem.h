#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"


class ComboUIScrollSystem
    : public ISystem {
private:
   
public:
    ComboUIScrollSystem();
    ~ComboUIScrollSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
