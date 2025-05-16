#include "BlockColorChangeSystem.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

#include "component/BigBom/BigBomStatus.h"
#include "component/Block/BlockCombinationStatus.h"
#include "component/Block/BlockManager.h"
#include "component/Block/BlockStatus.h"
#include "component/Bom/BomStatus.h"

BlockColorChangeSystem::BlockColorChangeSystem() : ISystem(SystemType::Collision) {}
BlockColorChangeSystem::~BlockColorChangeSystem() {}

void BlockColorChangeSystem::Initialize() {
}

void BlockColorChangeSystem::Finalize() {
}

void BlockColorChangeSystem::UpdateEntity(GameEntity* _entity) {

    if (!_entity) {
        return;
    }

    // CharacterStatusを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* combiEntity                  = ecsManager->getUniqueEntity("BlockCombination");

    BlockStatus* blockStatus                  = getComponent<BlockStatus>(_entity);
    ModelMeshRenderer* modelMesh              = getComponent<ModelMeshRenderer>(_entity);
    BlockCombinationStatus* combinationStatus = getComponent<BlockCombinationStatus>(combiEntity);

    if (!blockStatus) {
        return;
    }

    if (blockStatus->GetIsFall()) {
        return;
    }

    if (blockStatus->GetIsRedColor()) {
        switch (blockStatus->GetBlockType()) {
        case BlockType::NORMAL:
            modelMesh->getMaterialBuff(0)->color_ = blockStatus->GetNormalChangeColor();
            break;

        case BlockType::ADVANTAGE:
            modelMesh->getMaterialBuff(0)->color_ = blockStatus->GetAdvantageChangeColor();
            break;

        case BlockType::SKULL:
            modelMesh->getMaterialBuff(0)->color_ = blockStatus->GetSkullChangeColor();
            break;
        default:
            break;
        }
      
    } else {
        // 白
        modelMesh->getMaterialBuff(0)->color_ = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    }

    blockStatus->SetIsRedColor(false);

    /// ====================================================
    /// 衝突判定の結果を使って CharacterStatus を更新
    /// ====================================================
    {
        // sphereのみ
        std::vector<SphereCollider>* entityColliders = getComponents<SphereCollider>(_entity);
        for (auto& entityCollider : *entityColliders) {
            for (auto& [hitEntity, collisionState] : entityCollider.getCollisionStateMap()) {

                if (_entity->getDataType() == hitEntity->getDataType()) {
                    continue;
                }

                // 衝突開始時の オブジェクトの Lifeを減らす
                if (collisionState != CollisionState::Stay) {
                    continue;
                }

                // CharacterStatusを取得
                BomStatus* hitEntityStatus = getComponent<BomStatus>(hitEntity);
                /// BigBom
                BigBomStatus* bigbomStatus = getComponent<BigBomStatus>(hitEntity);

                SphereCollider* sphereCollider = getComponent<SphereCollider>(hitEntity, 1);

                if (!sphereCollider) {
                    return;
                }

                if (hitEntityStatus) {

                    modelMesh->getMaterialBuff(0)->color_ = blockStatus->GetNormalChangeColor();

                    if (blockStatus->GetBlockType() == BlockType::ADVANTAGE) {
                        // 右隣の壊れる予定のブロックを取得

                        if (combinationStatus) {
                            auto rightBlocks = combinationStatus->GetRightBlocksForCalucration(blockStatus->GetRowNum(), blockStatus->GetColumNum());
                            for (auto* rightBlock : rightBlocks) {
                                rightBlock->SetIsRedColor(true);
                            }
                        }
                    }

                } else if (bigbomStatus) {
                    if (bigbomStatus->GetIsLaunch()) {
                        modelMesh->getMaterialBuff(0)->color_ = blockStatus->GetNormalChangeColor();
                    }
                }
            }
        }
    }
}
