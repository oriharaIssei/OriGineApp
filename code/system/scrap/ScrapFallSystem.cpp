#include "ScrapFallSystem.h"

#define ENGINE_INCLUDE
#define ENGINE_COMPONENTS

#include "component/Scrap/ScrapStatus.h"
#include "engine/EngineInclude.h"

ScrapFallSystem::ScrapFallSystem()
    : ISystem(SystemType::Movement) {}

ScrapFallSystem::~ScrapFallSystem() {}

void ScrapFallSystem::Initialize() {}
void ScrapFallSystem::Finalize() {}

void ScrapFallSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity)
        return;

    ScrapStatus* scrapSystem = getComponent<ScrapStatus>(_entity); // status
    Rigidbody* rigitBody     = getComponent<Rigidbody>(_entity); // rigit
    Transform* transform     = getComponent<Transform>(_entity); // rigit
    if (!scrapSystem || !rigitBody || !transform) {
        return;
    }
  
    // 落ちてく(MovebyRigitBodyがやってくれてるでしょう)

    // 一定座標で止まる
    if (transform->translate[Y] <= scrapSystem->GetFallStopPosY()) {
        scrapSystem->SetIsStop(true);
    }

    if (!scrapSystem->GetIsStop()) {
        return;
    }

    // 止まる処理
    transform->translate[Y] = scrapSystem->GetFallStopPosY();
    rigitBody->setMass(0.0f);
    rigitBody->setUseGravity(false);
    rigitBody->setVelocity(Vec3f(0.0f, 0.0f, 0.0f));
}
