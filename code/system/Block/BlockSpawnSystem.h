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

class BlockSpawner;
class BlockSpawnSystem : public ISystem {
private:
    bool isInited_;
    BlockSpawner* blockSpawner_;
    Transform* lastTransform_ = nullptr;

public:
    BlockSpawnSystem();
    ~BlockSpawnSystem();

    void Initialize() override;
    void Finalize() override;

  
    void CreateBlocks(GameEntity* _entity, const int32_t& columIndex, const float& newPosX);


    void CostInit();

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
