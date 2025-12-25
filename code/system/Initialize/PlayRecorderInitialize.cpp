#include "PlayRecorderInitialize.h"

/// engine
#include "scene/Scene.h"

/// ECS
// component
#include "component/ghost/PlayRecordeComponent.h"

using namespace OriGine;

PlayRecorderInitialize::PlayRecorderInitialize() : ISystem(OriGine::SystemCategory::Initialize) {}
PlayRecorderInitialize::~PlayRecorderInitialize() = default;

void PlayRecorderInitialize::Initialize() {}
void PlayRecorderInitialize::Finalize() {}

void PlayRecorderInitialize::UpdateEntity(OriGine::EntityHandle _handle) {
    PlayRecordeComponent* playRecorder = GetComponent<PlayRecordeComponent>(_handle);

    if (!playRecorder) {
        return;
    }

    playRecorder->replayRecorder_ = std::make_shared<ReplayRecorder>();
    playRecorder->replayRecorder_->Initialize(GetScene()->GetName());
    // 最初のフレームを記録しておく
    playRecorder->replayRecorder_->RecordFrame(0.0f, GetScene()->GetKeyboardInput(), GetScene()->GetMouseInput(), GetScene()->GetGamepadInput());
}
