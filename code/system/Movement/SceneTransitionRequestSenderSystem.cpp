#include "SceneTransitionRequestSenderSystem.h"

/// engine
#include "messageBus/MessageBus.h"

/// event
#include "event/SceneChangeRequest.h"

/// ECS
// component
#include "component/SceneChanger.h"

using namespace OriGine;

SceneTransitionRequestSenderSystem::SceneTransitionRequestSenderSystem()
    : ISystem(SystemCategory::Movement) {}
SceneTransitionRequestSenderSystem::~SceneTransitionRequestSenderSystem() {}

void SceneTransitionRequestSenderSystem::Initialize() {}
void SceneTransitionRequestSenderSystem::Finalize() {}

void SceneTransitionRequestSenderSystem::UpdateEntity(OriGine::Entity* _entity) {
    auto* sceneChangerComps = GetComponents<SceneChanger>(_entity);

    if (!sceneChangerComps) {
        return;
    }

    for (auto& changer : *sceneChangerComps) {
        // シーン変更リクエストイベントを飛ばす
        if (changer.isChanged()) {
            MessageBus::GetInstance()->Emit<SceneChangeRequest>(SceneChangeRequest(changer.GetNextSceneName()));
        }
    }
}
