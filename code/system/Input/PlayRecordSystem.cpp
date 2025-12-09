#include "PlayRecordSystem.h"

/// engine
#define DELTA_TIME
#include "EngineInclude.h"

/// ECS
// component
#include "component/ghost/PlayRecordeComponent.h"

PlayRecordSystem::PlayRecordSystem() : ISystem(SystemCategory::Input) {}
PlayRecordSystem::~PlayRecordSystem() {}

void PlayRecordSystem::Initialize() {}
void PlayRecordSystem::Finalize() {}

void PlayRecordSystem::UpdateEntity(Entity* _entity) {
    auto recorder = GetComponent<PlayRecordeComponent>(_entity);

    if (!recorder) {
        return;
    }

    const auto keyInput     = GetScene()->GetKeyboardInput();
    const auto mouseInput   = GetScene()->GetMouseInput();
    const auto gamepadInput = GetScene()->GetGamepadInput();

    recorder->replayRecorder_->RecordFrame(
        GetMainDeltaTime(),
        keyInput,
        mouseInput,
        gamepadInput);
}
