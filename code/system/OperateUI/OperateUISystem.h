#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"
#include<array>
#include<string>

class OperateUISystem
    : public ISystem {
private:
   /* std::array<std::string, 10> textureName_;*/

public:
    OperateUISystem();
    ~OperateUISystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
