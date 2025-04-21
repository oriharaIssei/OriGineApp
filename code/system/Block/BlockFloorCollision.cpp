#include "BlockFloorCollision.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

#include"component/Piller/FloatingFloorStatus.h"
#include"component/Block/BlockStatus.h"
#include"component/Player/PlayerStates.h"

BlockFloorCollision::BlockFloorCollision() : ISystem(SystemType::Collision) {}
BlockFloorCollision::~BlockFloorCollision() {}

void BlockFloorCollision::Initialize() {

}

void BlockFloorCollision::Finalize() {

}


void BlockFloorCollision::UpdateEntity(GameEntity* _entity) {
  
    if (!_entity) {
        return;
    }

    // CharacterStatusを取得
 
    BlockStatus* blockStatus = getComponent<BlockStatus>(_entity);

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
                FloatingFloorStatus* hitEntityStatus = getComponent<FloatingFloorStatus>(hitEntity);
              

                if (!hitEntityStatus) {
                    continue;
                }

                if (!hitEntityStatus->GetIsFall()) {
                    continue;
                }

                // CharacterStatusを更新
                blockStatus->SetRatio(hitEntityStatus->GetRatio());
                blockStatus->SetIsBreak(true);
            }
        }
    }
}
