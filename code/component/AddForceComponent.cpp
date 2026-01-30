#include "AddForceComponent.h"

#ifdef _DEBUG
#include "myGui/MyGui.h"
#endif // _DEBUG

using namespace OriGine;

AddForceComponent::AddForceComponent() {}
AddForceComponent::~AddForceComponent() {}

void AddForceComponent::Initialize(Scene* /*_scene*/, EntityHandle /*_owner*/) {}
void AddForceComponent::Finalize() {}

void AddForceComponent::Edit(Scene* /*_scene*/, EntityHandle /*_owner*/, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG
    DragGuiCommand("Add Force##" + _parentLabel, addForce_, 0.01f);
#endif // _DEBUG
}

void to_json(nlohmann::json& j, const AddForceComponent& c) {
    j = nlohmann::json{
        {"addForce", c.addForce_},
    };
}

void from_json(const nlohmann::json& j, AddForceComponent& c) {
    j.at("addForce").get_to(c.addForce_);
}
