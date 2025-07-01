#include "ScrapToPlayerCollisionSystem.h"

#define ENGINE_INCLUDE
#define ENGINE_COMPONENTS

#include "component/Player/PlayerStates.h"
#include"component/Scrap/ScrapStatus.h"
#include "engine/EngineInclude.h"



ScrapToPlayerCollisionSystem::ScrapToPlayerCollisionSystem()
    : ISystem(SystemType::Collision) {}

ScrapToPlayerCollisionSystem::~ScrapToPlayerCollisionSystem() {}

void ScrapToPlayerCollisionSystem::Initialize() {}
void ScrapToPlayerCollisionSystem::Finalize() {}


void ScrapToPlayerCollisionSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

     ScrapStatus* scrapStaus = getComponent<ScrapStatus>(_entity);
     if (!scrapStaus) {
         return;
    }

     /// ====================================================
    /// 衝突判定の結果を使って CharacterStatus を更新
    /// ====================================================
    {
        // sphereのみ
        std::vector<SphereCollider>* entityColliders = getComponents<SphereCollider>(_entity);
        for (auto& entityCollider : *entityColliders) {
            for (auto& [hitEntityId, collisionState] : entityCollider.getCollisionStateMap()) {
                GameEntity* hitEntity = ECSManager::getInstance()->getEntity(hitEntityId);

                if (_entity->getDataType() == hitEntity->getDataType()) {
                    continue;
                }

                // 衝突開始時の オブジェクトの Lifeを減らす
                if (collisionState != CollisionState::Stay) {
                    continue;
                }

                // CharacterStatusを取得
                PlayerStates* hitEntityStatus = getComponent<PlayerStates>(hitEntity);
                if (!hitEntityStatus) {
                    continue;
                }

                if (hitEntityStatus->GetIsBigBomHaving()) {
                    continue;
                }

                // プレイヤーポイントアップ
                hitEntityStatus->IncrementCurrentBigBomPoint();
                // スクラップは死ぬ
                scrapStaus->SetIsDestroy(true);
            }
        }
    }
}

