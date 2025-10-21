#include "TimeLimitJudgeSystem.h"

// component
#include "component/SceneChanger.h"
#include "component/TimerComponent.h"

TimeLimitJudgeSystem::TimeLimitJudgeSystem() : ISystem(SystemCategory::StateTransition) {}

TimeLimitJudgeSystem::~TimeLimitJudgeSystem() {}

void TimeLimitJudgeSystem::Initialize() {}
void TimeLimitJudgeSystem::Finalize() {}

void TimeLimitJudgeSystem::UpdateEntity(Entity* _entity) {
    auto timerComp = getComponent<TimerComponent>(_entity);
    if (!timerComp) {
        return;
    }
    if (timerComp->getCurrentTime() <= -0.0001f) {
        SceneChanger* sceneChanger = getComponent<SceneChanger>(_entity);
        if (!sceneChanger) {
            return;
        }
        sceneChanger->ChangeScene();
        return;
    }
}
