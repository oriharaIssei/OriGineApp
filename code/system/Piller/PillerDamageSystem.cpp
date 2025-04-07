#include "PillerDamageSystem.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

#include"component/Piller/PillerStates.h"
#include"component/Bom/ExplotionCollision.h"

PillerDamageSystem::PillerDamageSystem() : ISystem(SystemType::Collision) {}
PillerDamageSystem::~PillerDamageSystem() {}

void PillerDamageSystem::Initialize() {

}

void PillerDamageSystem::Finalize() {

}


void PillerDamageSystem::UpdateEntity(GameEntity* _entity) {
  
    if (!_entity) {
        return;
    }

    // CharacterStatusを取得
    PillerStates* entityStatus = getComponent<PillerStates>(_entity);

    if (!entityStatus) {
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
                if (collisionState != CollisionState::Stay) {
                    continue;
                }
                // CharacterStatusを取得
                ExplotionCollision* hitEntityStatus = getComponent<ExplotionCollision>(hitEntity);
                if (!hitEntityStatus) {
                    continue;
                }
                // CharacterStatusを更新
                entityStatus->TakeDamage();
            }
        }
    }
}
