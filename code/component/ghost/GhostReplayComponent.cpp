#include "GhostReplayComponent.h"

using namespace OriGine;

void GhostReplayComponent::Initialize(Scene* /*_scene*/, EntityHandle /*_owner*/) {}
void GhostReplayComponent::Finalize() {}

void GhostReplayComponent::Edit(OriGine::Scene* /*_scene*/, OriGine::Entity* /*_OriGine::Entity*/, const std::string& /*_parentLabel*/) {
#ifdef _DEBUG

#endif // _DEBUG
}

float GhostReplayComponent::ApplyInput() {
    if (replayPlayer_) {
        // リプレイデータを適用
        return replayPlayer_->Apply(keyboardInput_.get(), mouseInput_.get(), gamepadInput_.get());
    }
    return 0.0f;
}

void to_json(nlohmann::json& /*_j*/, const GhostReplayComponent& /*_c*/) {}
void from_json(const nlohmann::json& /*_j*/, GhostReplayComponent& /*_c*/) {}
