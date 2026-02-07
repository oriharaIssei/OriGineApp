#include "VelocityOverrideComponent.h"

#ifdef _DEBUG
#include "myGui/MyGui.h"
#endif // _DEBUG

using namespace OriGine;

VelocityOverrideComponent::VelocityOverrideComponent() {}
VelocityOverrideComponent::~VelocityOverrideComponent() {}

void VelocityOverrideComponent::Initialize(OriGine::Scene* /*_scene*/, OriGine::EntityHandle /*_owner*/) {}
void VelocityOverrideComponent::Finalize() {}

void VelocityOverrideComponent::Edit(OriGine::Scene* /*_scene*/, OriGine::EntityHandle /*_owner*/, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG
    DragGuiVectorCommand("Forced Movement Vector##" + _parentLabel, forcedMovementVector_, 0.01f);
    DragGuiCommand("Stun Time##" + _parentLabel, stunTime_, 0.01f);
#endif // _DEBUG
}

void to_json(nlohmann::json& j, const VelocityOverrideComponent& c) {
    j = nlohmann::json{
        {"forcedMovementVector", c.forcedMovementVector_},
        {"stunTime", c.stunTime_},
    };
}

void from_json(const nlohmann::json& j, VelocityOverrideComponent& c) {
    j.at("forcedMovementVector").get_to(c.forcedMovementVector_);
    j.at("stunTime").get_to(c.stunTime_);
}
