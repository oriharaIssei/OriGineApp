#include "PlayRecorderComponent.h"

using namespace OriGine;

PlayRecorderComponent::PlayRecorderComponent() {}
PlayRecorderComponent::~PlayRecorderComponent() {}

void PlayRecorderComponent::Initialize(Scene* /*_scene*/, EntityHandle /*_owner*/) {}
void PlayRecorderComponent::Finalize() {}

void PlayRecorderComponent::Edit(Scene* /*_scene*/, EntityHandle /*_handle*/, const std::string& /*_parentLabel*/) {}

void to_json(nlohmann::json& /*_j*/, const PlayRecorderComponent& /*_c*/) {
}

void from_json(const nlohmann::json& /*_j*/, PlayRecorderComponent& /*_c*/) {
}
