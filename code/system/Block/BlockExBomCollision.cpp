#include "BlockExBomCollision.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

#include "component/Block/BlockManager.h"
#include "component/Block/BlockStatus.h"
#include "component/Bom/ExplotionCollision.h"
#include "component/Player/PlayerStates.h"

BlockExBomCollision::BlockExBomCollision() : ISystem(SystemType::Collision) {}
BlockExBomCollision::~BlockExBomCollision() {}

void BlockExBomCollision::Initialize() {
}

void BlockExBomCollision::Finalize() {
}

void BlockExBomCollision::UpdateEntity(GameEntity* _entity) {

    if (!_entity) {
        return;
    }

    // CharacterStatusを取得

    BlockStatus* blockStatus                 = getComponent<BlockStatus>(_entity);
  

    if (!blockStatus) {
        return;
    }

    if (blockStatus->GetIsFall()) {
        return;
    }

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
                if (collisionState != CollisionState::Enter) {
                    continue;
                }

                // CharacterStatusを取得
                ExplotionCollision* hitEntityStatus = getComponent<ExplotionCollision>(hitEntity);
                PlayerStates* playerStatus          = getComponent<PlayerStates>(hitEntity);

                if (!hitEntityStatus || playerStatus) {
                    continue;
                }
                // CharacterStatusを更新
                blockStatus->SetIsBreak(true);

              
            }
        }
    }
}
