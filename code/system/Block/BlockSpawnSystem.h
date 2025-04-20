// FloorSystem.h
#pragma once
#include "system/ISystem.h"

#include <component/transform/Transform.h>
#include <cstdint>
#include <vector>
#include <Vector3.h>

///====================================================================
// FloorSystem
///====================================================================

class BlockManager;
class BlockSpawnSystem : public ISystem {
private:
    bool isInited_;
    BlockManager* blockSpawner_;
    Transform* lastTransform_ = nullptr;

public:
    BlockSpawnSystem();
    ~BlockSpawnSystem();

    void Initialize() override;
    void Finalize() override;

  
    void CreateBlocks(const int32_t& columIndex, const float& newPosX);


    void CostInit();

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
