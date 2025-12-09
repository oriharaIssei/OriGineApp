#include "PlayRecorderInitialize.h"

/// ECS
// component
#include "component/ghost/PlayRecordeComponent.h"

PlayRecorderInitialize::PlayRecorderInitialize() : ISystem(SystemCategory::Initialize) {}
PlayRecorderInitialize::~PlayRecorderInitialize() = default;

void PlayRecorderInitialize::Initialize() {}
void PlayRecorderInitialize::Finalize() {}

void PlayRecorderInitialize::UpdateEntity(Entity* _entity) {
    PlayRecordeComponent* playRecorder = GetComponent<PlayRecordeComponent>(_entity);

    if (!playRecorder) {
        return;
    }

    playRecorder->replayRecorder_ = std::make_shared<ReplayRecorder>();
    playRecorder->replayRecorder_->Initialize(GetScene()->GetName());
    // 最初のフレームを記録しておく
    playRecorder->replayRecorder_->RecordFrame(0.0f, GetScene()->GetKeyboardInput(), GetScene()->GetMouseInput(), GetScene()->GetGamepadInput());
}
