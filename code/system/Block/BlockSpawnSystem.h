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
class BlockCombinationStatus;
class BlockSpawnSystem : public ISystem {
private:
    bool isInited_;
    BlockManager* blockSpawner_;
    Transform* lastTransform_ = nullptr;
    BlockCombinationStatus* blockCombinationStatus_ = nullptr;
    // Advanceブロック生成colum予約
    std::vector<int32_t> reservedSkullColumns_; // 追加
    int32_t nextSpecialLine_;

public:
    BlockSpawnSystem();
    ~BlockSpawnSystem();

    void Initialize() override;
    void Finalize() override;

  
    void CreateBlocks(const int32_t& columnIndex, const int32_t& rowIndex, const float& xPos);
    void BlockTypeSetting(BlockStatus* status, BlockType blocktype);
    void ModelSetForBlockType(ModelMeshRenderer* render, GameEntity* entity,BlockType type);

    void CostInit();
    void BlockTypeSettingBySameColum(BlockStatus* status, const int32_t& columnNum);

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
