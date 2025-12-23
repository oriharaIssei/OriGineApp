#include "TimeLimitJudgeSystem.h"

/// engine
#include "messageBus/MessageBus.h"

/// event
#include "event/GamefailedEvent.h"

/// ECS
// component
#include "component/SceneChanger.h"
#include "component/TimerComponent.h"

/// math
#include "math/mathEnv.h"

using namespace OriGine;

TimeLimitJudgeSystem::TimeLimitJudgeSystem() : ISystem(OriGine::SystemCategory::StateTransition) {}
TimeLimitJudgeSystem::~TimeLimitJudgeSystem() {}

void TimeLimitJudgeSystem::Initialize() {}
void TimeLimitJudgeSystem::Finalize() {}

void TimeLimitJudgeSystem::UpdateEntity(OriGine::Entity* _entity) {
    auto timerComp = GetComponent<TimerComponent>(_entity);
    if (!timerComp) {
        return;
    }
    if (timerComp->GetTime() <= -kEpsilon) {
        MessageBus::GetInstance()->Emit<GamefailedEvent>(GamefailedEvent());
        return;
    }
}
