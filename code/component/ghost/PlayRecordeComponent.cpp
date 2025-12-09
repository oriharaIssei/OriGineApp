#include "PlayRecordeComponent.h"

PlayRecordeComponent::PlayRecordeComponent() {}
PlayRecordeComponent::~PlayRecordeComponent() {}

void PlayRecordeComponent::Initialize(Entity* /*_entity*/) {}
void PlayRecordeComponent::Finalize() {}

void PlayRecordeComponent::Edit(Scene* /*_scene*/, Entity* /*_entity*/, const std::string& /*_parentLabel*/) {}

void to_json(nlohmann::json& /*_j*/, const PlayRecordeComponent& /*_c*/) {
}

void from_json(const nlohmann::json& /*_j*/, PlayRecordeComponent& /*_c*/) {
}
