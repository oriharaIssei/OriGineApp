#include "BlockCombinationStatus.h"
#include "component/transform/Transform.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"
#include <string>

void BlockCombinationStatus::Initialize([[maybe_unused]] GameEntity* _entity) {

  
}

bool BlockCombinationStatus::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", isAlive_);
    isChange |= InputGuiCommand("conbiMax", conbinationMax_);
  

    ImGui::Spacing();

    return isChange;
}

void BlockCombinationStatus::Finalize() {}

void BlockCombinationStatus::AddBlockStatus(BlockStatus* status) {
    blockStatusArray_.push_back(status);
}

void to_json(nlohmann::json& _json, const BlockCombinationStatus& _block) {
    _json["isAlive"] = _block.isAlive_;
    _json["conbiMax"] = _block.conbinationMax_;
}

void from_json(const nlohmann::json& _json, BlockCombinationStatus& _block) {
    _json.at("isAlive").get_to(_block.isAlive_);
    if (auto it = _json.find("conbiMax"); it != _json.end()) {
        _json.at("conbiMax").get_to(_block.conbinationMax_);
    }
}

