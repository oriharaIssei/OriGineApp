// FloorSystem.h
#pragma once
#include "system/ISystem.h"

#include "component/Block/BlockManager.h"
#include <cstdint>


///====================================================================
// FloorSystem
///====================================================================

class FuseAdaptPosSystem : public ISystem {
private:
    bool isInited_;

 
    public:
    FuseAdaptPosSystem();
    ~FuseAdaptPosSystem();

    void Initialize() override;
    void Finalize() override;

 
   
protected:
    void UpdateEntity(GameEntity* _entity) override;
};
