#include "FloatingFloorDamageSystem.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

#include "component/Bom/ExplotionCollision.h"
#include"component/BigBom/BigExplotionCollision.h"
#include "component/Piller/FloatingFloorStatus.h"
#include "component/Player/PlayerStates.h"

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

      EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
   
    FloatingFloorStatus* floatingFloorStatus = getComponent<FloatingFloorStatus>(_entity);
    GameEntity* FloorSound                   = ecsManager->getUniqueEntity("FloorSound");
    Audio* damageSound                       = getComponent<Audio>(FloorSound, 4);
    if (!floatingFloorStatus || !damageSound) {
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
                BigExplotionCollision* hitEntityBigStatus = getComponent<BigExplotionCollision>(hitEntity);
                PlayerStates* playerStatus          = getComponent<PlayerStates>(hitEntity);

                if (playerStatus || (!hitEntityBigStatus && !hitEntityStatus)) {
                    continue;
                }
                //ダメ―ジ音
                damageSound->Play();

                if (hitEntityBigStatus) {
                    float ratio = floatingFloorStatus->GetScoreUPRatio() * hitEntityBigStatus->GetPlusScoreRatio();
                    floatingFloorStatus->SetScoreUpRatio(ratio);
                    floatingFloorStatus->TakeBigDamage();
                } else {
                    floatingFloorStatus->TakeDamage();
                }
            }
        }
    }
}
