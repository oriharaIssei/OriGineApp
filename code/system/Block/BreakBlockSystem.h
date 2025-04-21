#pragma once

#include "system/ISystem.h"
#include"component/Block/BlockManager.h"

class BreakBlockSystem
    : public ISystem {
public:
    BreakBlockSystem();
    ~BreakBlockSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

    void BlockReaction(BlockType blocktype);
   

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
   
};
