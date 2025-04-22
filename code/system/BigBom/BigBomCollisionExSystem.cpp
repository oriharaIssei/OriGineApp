#include "BigBomCollisionExSystem.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

#include"component/Bom/BomStatus.h"
#include"component/Bom/ExplotionCollision.h"
#include"component/Piller/FloatingFloorStatus.h"

BigBomCollisionExSystem::BigBomCollisionExSystem() : ISystem(SystemType::Collision) {}
BigBomCollisionExSystem::~BigBomCollisionExSystem() {}

void BigBomCollisionExSystem::Initialize() {

}

void BigBomCollisionExSystem::Finalize() {

}


void BigBomCollisionExSystem::UpdateEntity(GameEntity* _entity) {
  
    if (!_entity) {
        return;
    }

    // CharacterStatusを取得
 
    BomStatus* bomStatus= getComponent<BomStatus>(_entity);

    if (!bomStatus) {
        return;
    }

    if (bomStatus->GetIsExplotion()) {
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
                // CharacterStatusを更新
                bomStatus->SetIsExplotion(true);
            }
        }
    }
}
