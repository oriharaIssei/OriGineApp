#include "PlayRecordSystem.h"

/// engine
#define DELTA_TIME
#include "EngineInclude.h"
#include "scene/Scene.h"

/// ECS
// component
#include "component/ghost/PlayRecordeComponent.h"

using namespace OriGine;

PlayRecordSystem::PlayRecordSystem() : ISystem(OriGine::SystemCategory::Input) {}
PlayRecordSystem::~PlayRecordSystem() {}

void PlayRecordSystem::Initialize() {}
void PlayRecordSystem::Finalize() {}

void PlayRecordSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    auto recorder = GetComponent<PlayRecordeComponent>(_handle);

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
