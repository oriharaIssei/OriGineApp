#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class BlockMoveSystem
    : public ISystem {
private:
   
public:
    BlockMoveSystem();
    ~BlockMoveSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;;
  
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
