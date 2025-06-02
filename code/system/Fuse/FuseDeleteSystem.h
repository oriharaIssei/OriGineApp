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
class FuseDeleteSystem : public ISystem {
private:
    bool isInited_;

public:
    FuseDeleteSystem();
    ~FuseDeleteSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
