#include "BlockCombinationStatus.h"
#include "BlockStatus.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"
#include <algorithm>
#include <string>

void BlockCombinationStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool BlockCombinationStatus::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", isAlive_);
    isChange |= InputGuiCommand("conbiMax", conbinationMax_);
    isChange |= DragGuiCommand("plusTimerRate", plusTimerRate_);
    isChange |= DragGuiCommand("plusTimerValue", plusTimerValue_);
    isChange |= DragGuiCommand("plusScoreValue", plusScoreValue_);
    isChange |= DragGuiCommand("plusScoreRate", plusScoreRate_);
    isChange |= DragGuiCommand("breakOffsetTime", breakOffsetTime_);
    ImGui::Spacing();

    return isChange;
}

void BlockCombinationStatus::Finalize() {}

void BlockCombinationStatus::AddBlockStatus(BlockStatus* status) {
    blockStatusArray_.push_back(status);
}

void to_json(nlohmann::json& _json, const BlockCombinationStatus& _block) {
    _json["isAlive"]         = _block.isAlive_;
    _json["conbiMax"]        = _block.conbinationMax_;
    _json["plusRate"]        = _block.plusTimerRate_;
    _json["plusValue"]       = _block.plusTimerValue_;
    _json["breakOffsetTime"] = _block.breakOffsetTime_;
    _json["plusScoreRate"]   = _block.plusScoreRate_;
    _json["plusScoreValue"]  = _block.plusScoreValue_;
}

void from_json(const nlohmann::json& _json, BlockCombinationStatus& _block) {
    _json.at("isAlive").get_to(_block.isAlive_);
    if (auto it = _json.find("conbiMax"); it != _json.end()) {
        _json.at("conbiMax").get_to(_block.conbinationMax_);
    }

    if (auto it = _json.find("plusRate"); it != _json.end()) {
        _json.at("plusRate").get_to(_block.plusTimerRate_);
    }

    if (auto it = _json.find("plusValue"); it != _json.end()) {
        _json.at("plusValue").get_to(_block.plusTimerValue_);
    }

    if (auto it = _json.find("breakOffsetTime"); it != _json.end()) {
        _json.at("breakOffsetTime").get_to(_block.breakOffsetTime_);
    }

    if (auto it = _json.find("plusScoreRate"); it != _json.end()) {
        _json.at("plusScoreRate").get_to(_block.plusScoreRate_);
    }

    if (auto it = _json.find("plusScoreValue"); it != _json.end()) {
        _json.at("plusScoreValue").get_to(_block.plusScoreValue_);
    }
}

std::vector<BlockStatus*> BlockCombinationStatus::GetRightBlocks(const int& baseRowNum, const int& columNum) {
    std::vector<BlockStatus*> result;
    for (BlockStatus* status : blockStatusArray_) {
        int row = status->GetColum();

        // colum が血がければskip
        if (columNum != status->GetRow()) {
            continue;
        }

        if (row < baseRowNum && row >= conbinationMax_) {
            result.push_back(status);
        }
    }
    // rowNumが小さい順にソート（右から順）
    std::sort(result.begin(), result.end(), [](BlockStatus* a, BlockStatus* b) {
        return a->GetColum() > b->GetColum();
    });

   // 最後尾にフラグを立てる
  /*  if (!result.empty()) {
        result.back()->SetIsRightEdge(true);
    }*/
   
    return result;
}
