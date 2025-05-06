#include "BlockColorChangeSystem.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

#include "component/Block/BlockManager.h"
#include "component/Block/BlockStatus.h"
#include"component/Bom/BomStatus.h"
#include "component/Bom/ExplotionCollision.h"
#include "component/Player/PlayerStates.h"

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

    BlockStatus* blockStatus     = getComponent<BlockStatus>(_entity);
    ModelMeshRenderer* modelMesh = getComponent<ModelMeshRenderer>(_entity);
    /* ModelMeshRenderer* modelMesh = getComponent<ModelMeshRenderer>(_entity);*/

    if (!blockStatus) {
        return;
    }

    if (blockStatus->GetIsFall()) {
        return;
    }

    //白
     modelMesh->getMaterialBuff(0)->color_ = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);

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
                BomStatus* hitEntityStatus           = getComponent<BomStatus>(hitEntity);
                SphereCollider* preBomshepreCollider = getComponent<SphereCollider>(hitEntity, 1);

                if (!hitEntityStatus || !preBomshepreCollider) {
                    continue;
                }

                modelMesh->getMaterialBuff(0)->color_ = blockStatus->GetChangeColor();
                
            }
        }
    }
}
