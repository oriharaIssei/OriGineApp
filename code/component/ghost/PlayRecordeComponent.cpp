#include "PlayRecordeComponent.h"

PlayRecordeComponent::PlayRecordeComponent() {}
PlayRecordeComponent::~PlayRecordeComponent() {}

void PlayRecordeComponent::Initialize(OriGine::Entity* /*_OriGine::Entity*/) {}
void PlayRecordeComponent::Finalize() {}

void PlayRecordeComponent::Edit(OriGine::Scene* /*_scene*/, OriGine::Entity* /*_OriGine::Entity*/, const std::string& /*_parentLabel*/) {}

void to_json(nlohmann::json& /*_j*/, const PlayRecordeComponent& /*_c*/) {
}

void from_json(const nlohmann::json& /*_j*/, PlayRecordeComponent& /*_c*/) {
}
