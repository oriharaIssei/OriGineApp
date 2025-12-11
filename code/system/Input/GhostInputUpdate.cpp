#include "GhostInputUpdate.h"

/// engine
#define DELTA_TIME
#define ENGINE_INCLUDE
#include "EngineInclude.h"

/// ECS
// component
#include "component/camera/CameraController.h"
#include "component/ghost/GhostReplayComponent.h"
#include "component/MouseCondition.h"
#include "component/player/PlayerInput.h"
#include "component/player/State/PlayerState.h"

// system
#include "system/SystemRunner.h"

#include "system/Movement/StartSequenceSystem.h"

GhostInputUpdate::GhostInputUpdate() : ISystem(OriGine::SystemCategory::Input) {}
GhostInputUpdate::~GhostInputUpdate() {}

void GhostInputUpdate::Initialize() {
    playerInputSystem_ = std::make_unique<PlayerInputSystem>();
    cameraInputSystem_ = std::make_unique<CameraInputSystem>();

    playerInputSystem_->SetScene(GetScene());
    cameraInputSystem_->SetScene(GetScene());
}
void GhostInputUpdate::Finalize() {}

void GhostInputUpdate::UpdateEntity(OriGine::Entity* _entity) {

    auto comp = GetComponent<GhostReplayComponent>(_entity);
    if (!comp || !comp->replayPlayer_) {
        return;
    }

    auto player = comp->replayPlayer_.get();
    if (!player->GetIsActive() || player->IsEnd()) {
        return;
    }

    int32_t current = static_cast<int32_t>(player->GetCurrentFrameIndex());

    if (!player->Seek(current + 1)) {
        return;
    }

    float newDelta = player->Apply(
        comp->keyboardInput_.get(),
        comp->mouseInput_.get(),
        comp->gamepadInput_.get());

    // TODO:: 禁忌
    OriGine::Engine::GetInstance()->SetDeltaTime(newDelta);

    MouseCondition* mouseCondition =
        GetComponent<MouseCondition>(GetEntity(comp->ghostCameraEntityId_));
    if (mouseCondition) {
        comp->mouseInput_->SetPosition(mouseCondition->GetFixCursorPos());
    }

    playerInputSystem_->InputUpdate(
        newDelta,
        comp->keyboardInput_.get(),
        comp->gamepadInput_.get(),
        GetComponent<PlayerInput>(GetEntity(comp->ghostEntityId_)),
        GetComponent<PlayerState>(GetEntity(comp->ghostEntityId_)));

    cameraInputSystem_->InputUpdate(
        newDelta,
        comp->mouseInput_.get(),
        comp->gamepadInput_.get(),
        GetComponent<CameraController>(GetEntity(comp->ghostCameraEntityId_)));
}
