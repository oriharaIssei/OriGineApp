#include "FloatingFloorDamageSystem.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

#include"component/Bom/ExplotionCollision.h"
#include"component/Piller/FloatingFloorStatus.h"

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
 
    FloatingFloorStatus* fAndPStatus = getComponent<FloatingFloorStatus>(_entity);

    if (!fAndPStatus) {
        return;
    }

    if (fAndPStatus->GetIsFall()) {
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
                if (!hitEntityStatus) {
                    continue;
                }
                // CharacterStatusを更新
                fAndPStatus->TakeDamage();
            }
        }
    }
}
