// FloorSystem.h
#pragma once
#include "system/ISystem.h"

#include <component/transform/Transform.h>
#include "component/Block/BlockManager.h"
#include <cstdint>
#include <vector>
#include <Vector3.h>


///====================================================================
// FloorSystem
///====================================================================
class ModelMeshRenderer;
class BlockManager;
class BlockStatus;
class BlockSpawnSystem : public ISystem {
private:
    bool isInited_;
    BlockManager* blockSpawner_;
    Transform* lastTransform_ = nullptr;

   
    int32_t nextSpecialLine_;

public:
    BlockSpawnSystem();
    ~BlockSpawnSystem();

    void Initialize() override;
    void Finalize() override;

  
    void CreateBlocks(const int32_t& columIndex, const float& newPosX);
    void BlockTypeSetting(BlockStatus* status, BlockType blocktype);
    void ModelSetForBlockType(ModelMeshRenderer* render, GameEntity* entity,BlockType type);

    void CostInit();

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
