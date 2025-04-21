#pragma once

#include "component/Block/BlockManager.h"
#include "system/ISystem.h"

 class BlockStatus;

class DeleteBlockSystem
    : public ISystem {
public:
    DeleteBlockSystem();
    ~DeleteBlockSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;

    void BlockReaction(BlockType blockType);

    private:
    BlockStatus* blockStatus_;
   
};
