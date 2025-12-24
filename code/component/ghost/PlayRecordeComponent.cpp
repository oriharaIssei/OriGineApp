#include "PlayRecordeComponent.h"

using namespace OriGine;

PlayRecordeComponent::PlayRecordeComponent() {}
PlayRecordeComponent::~PlayRecordeComponent() {}

void PlayRecordeComponent::Initialize(Scene* /*_scene*/, EntityHandle /*_owner*/) {}
void PlayRecordeComponent::Finalize() {}

void PlayRecordeComponent::Edit(Scene* /*_scene*/, EntityHandle /*_handle*/, const std::string& /*_parentLabel*/) {}

void to_json(nlohmann::json& /*_j*/, const PlayRecordeComponent& /*_c*/) {
}

void from_json(const nlohmann::json& /*_j*/, PlayRecordeComponent& /*_c*/) {
}
