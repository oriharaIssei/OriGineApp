#include "CharacterOnCollision.h"

/// engine
/// ECS
#define ENGINE_ECS
#include "EngineInclude.h"
/// app
// component
#include "component/CharacterStatus.h"

CharacterOnCollision::CharacterOnCollision() : ISystem(SystemType::Collision) {}

CharacterOnCollision::~CharacterOnCollision() {
}

void CharacterOnCollision::Initialize() {}

void CharacterOnCollision::Finalize() {}

void CharacterOnCollision::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    // CharacterStatusを取得
    CharacterStatus* entityStatus = getComponent<CharacterStatus>(_entity);
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
                if (collisionState != CollisionState::Enter) {
                    continue;
                }
                // CharacterStatusを取得
                CharacterStatus* hitEntityStatus = getComponent<CharacterStatus>(hitEntity);
                if (!hitEntityStatus) {
                    continue;
                }
                // CharacterStatusを更新
                entityStatus->takeDamage(hitEntityStatus->getAttack());
            }
        }
    }
}
