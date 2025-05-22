#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class BlockApearSystem
    : public ISystem {
private:
  
    float time_;

public:
    BlockApearSystem();
    ~BlockApearSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;


};
