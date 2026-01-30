#include "PlayRecordSystem.h"

/// engine
#include "Engine.h"
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

    // 各入力を取得 ＆ 記録
    const auto keyInput     = GetScene()->GetKeyboardInput();
    const auto mouseInput   = GetScene()->GetMouseInput();
    const auto gamepadInput = GetScene()->GetGamepadInput();

    float deltaTime = Engine::GetInstance()->GetDeltaTime();

    recorder->replayRecorder_->RecordFrame(
        deltaTime,
        keyInput,
        mouseInput,
        gamepadInput);
}
