#include "SceneTransitionRequestReceiverSystem.h"

/// engine
#include "messageBus/MessageBus.h"

/// event
#include "event/SceneChangeRequest.h"

/// ECS
// component
#include "component/SceneChanger.h"

using namespace OriGine;

SceneTransitionRequestReceiverSystem::SceneTransitionRequestReceiverSystem()
    : ISystem(SystemCategory::Movement) {}
SceneTransitionRequestReceiverSystem::~SceneTransitionRequestReceiverSystem() {}

void SceneTransitionRequestReceiverSystem::Initialize() {
    MessageBus::GetInstance()->Subscribe<SceneChangeRequest>(
        [this](const SceneChangeRequest& _event) {
            isSceneChangeRequested_ = true;
            targetSceneName_        = _event.targetSceneName;
        });
}
void SceneTransitionRequestReceiverSystem::Finalize() {
    MessageBus::GetInstance()->Unsubscribe<SceneChangeRequest>(sceneChangeRequestEventId_);
}

void SceneTransitionRequestReceiverSystem::UpdateEntity(OriGine::Entity* _entity) {
    if (!isSceneChangeRequested_) {
        return;
    }
    // フラグをリセット
    isSceneChangeRequested_ = false;

    auto* sceneChangerComp = GetComponent<SceneChanger>(_entity);

    if (!sceneChangerComp) {
        return;
    }

    // シーン変更リクエストイベントを飛ばす
    sceneChangerComp->SetNextSceneName(targetSceneName_);
    sceneChangerComp->ChangeScene();
}
