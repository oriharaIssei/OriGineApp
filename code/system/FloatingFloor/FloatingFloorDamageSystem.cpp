#include "FloatingFloorDamageSystem.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

#include"component/Bom/ExplotionCollision.h"
#include"component/Piller/FloatingFloorStatus.h"
#include"component/Player/PlayerStates.h"

FloatingFloorDamageSystem::FloatingFloorDamageSystem() : ISystem(SystemType::Collision) {}
FloatingFloorDamageSystem::~FloatingFloorDamageSystem() {}

void FloatingFloorDamageSystem::Initialize() {

}

void FloatingFloorDamageSystem::Finalize() {

}


void FloatingFloorDamageSystem::UpdateEntity(GameEntity* _entity) {
  
    if (!_entity) {
        return;
    }

    // CharacterStatusを取得
 
    FloatingFloorStatus* floatingFloorStatus = getComponent<FloatingFloorStatus>(_entity);

    if (!floatingFloorStatus) {
        return;
    }

    if (floatingFloorStatus->GetIsFall() || floatingFloorStatus->GetIsDestroy()) {
        return;
    }
   
    /// ====================================================
    /// 衝突判定の結果を使って CharacterStatus を更新
    /// ====================================================
    {
        // sphereのみ
        std::vector<AABBCollider>* entityColliders = getComponents<AABBCollider>(_entity);
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
                floatingFloorStatus->TakeDamage();
            }
        }
    }
}
