#include "Stage.h"

/// stl
#include <unordered_map>

#ifdef DEBUG
#include "myGui/MyGui.h"
#endif // DEBUG

void Stage::Initialize(GameEntity* /*_entity*/) {}

void Stage::Edit(Scene* /*_scene*/, GameEntity* /*_entity*/, [[maybe_unused]] const std::string& _parentLabel) {

#ifdef DEBUG

#endif
}

void Stage::Finalize() {
    controlPoints_.clear();
    links_.clear();
}

void to_json(nlohmann::json& /*j*/, const Stage& /*stage*/) {}

void from_json(const nlohmann::json& /*j*/, Stage& /*stage*/) {}
