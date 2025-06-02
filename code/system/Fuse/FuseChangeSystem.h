// FloorSystem.h
#pragma once
#include "system/ISystem.h"

#include "component/Block/BlockManager.h"
#include <cstdint>


///====================================================================
// FloorSystem
///====================================================================
class BlockStatus;
class BlockCombinationStatus;
class FuseChangeSystem : public ISystem {
private:
    bool isInited_;

    BlockManager* blockSpawner_=nullptr;
    BlockCombinationStatus* blockCombinationStatus_ = nullptr;

    public:
    FuseChangeSystem();
        ~FuseChangeSystem();

    void Initialize() override;
    void Finalize() override;

    void CreateFuse(GameEntity* _entity, BlockStatus*blockStatus);
   
   
protected:
    void UpdateEntity(GameEntity* _entity) override;
};
