#include "TimeLimitJudgeSystem.h"

// component
#include "component/SceneChanger.h"
#include "component/TimerComponent.h"

///math
#include "math/mathEnv.h"

TimeLimitJudgeSystem::TimeLimitJudgeSystem() : ISystem(SystemCategory::StateTransition) {}

TimeLimitJudgeSystem::~TimeLimitJudgeSystem() {}

void TimeLimitJudgeSystem::Initialize() {}
void TimeLimitJudgeSystem::Finalize() {}

void TimeLimitJudgeSystem::UpdateEntity(Entity* _entity) {
    auto timerComp = GetComponent<TimerComponent>(_entity);
    if (!timerComp) {
        return;
    }
    if (timerComp->GetCurrentTime() <= -kEpsilon) {
        SceneChanger* sceneChanger = GetComponent<SceneChanger>(_entity);
        if (!sceneChanger) {
            return;
        }
        sceneChanger->ChangeScene();
        return;
    }
}
