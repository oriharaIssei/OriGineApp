#include "BlockCombinationStatus.h"
#include "BlockStatus.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"
#include <algorithm>
#include <vector>

void BlockCombinationStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool BlockCombinationStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG
    isChange = CheckBoxCommand("IsAlive", isAlive_);
    isChange |= InputGuiCommand("conbiMax", conbinationMax_);
    isChange |= DragGuiCommand("plusTimerRate", plusTimerRate_);
    isChange |= DragGuiCommand("plusTimerValue", plusTimerValue_);
    isChange |= DragGuiCommand("plusScoreValue", plusScoreValue_);
    isChange |= DragGuiCommand("plusScoreRate", plusScoreRate_);
    isChange |= DragGuiCommand("breakOffsetTime", breakOffsetTime_);

    isChange |= DragVectorGui("timerUIPosOffset", timerUIPosOffset_);
    isChange |= DragVectorGui("scoreUIPosOffset", scoreUIPosOffset_);
    ImGui::Spacing();
#endif // _DEBUG

    return isChange;
}

void BlockCombinationStatus::Finalize() {}

void BlockCombinationStatus::AddBlockStatus(BlockStatus* status) {
    blockStatusArray_.push_back(status);
}

void to_json(nlohmann::json& _json, const BlockCombinationStatus& _block) {
    _json["isAlive"]          = _block.isAlive_;
    _json["conbiMax"]         = _block.conbinationMax_;
    _json["plusRate"]         = _block.plusTimerRate_;
    _json["plusValue"]        = _block.plusTimerValue_;
    _json["breakOffsetTime"]  = _block.breakOffsetTime_;
    _json["plusScoreRate"]    = _block.plusScoreRate_;
    _json["plusScoreValue"]   = _block.plusScoreValue_;
    _json["timerUIPosOffset"] = _block.timerUIPosOffset_;
    _json["scoreUIPosOffset"] = _block.scoreUIPosOffset_;
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
    if (auto it = _json.find("timerUIPosOffset"); it != _json.end()) {
        _json.at("timerUIPosOffset").get_to(_block.timerUIPosOffset_);
    }
    if (auto it = _json.find("scoreUIPosOffset"); it != _json.end()) {
        _json.at("scoreUIPosOffset").get_to(_block.scoreUIPosOffset_);
    }
}

bool BlockCombinationStatus::JudgeIsAdvantageToLeft(const int& baseRowNum, const int& columNum) const {
    std::vector<BlockStatus*> result;

    for (BlockStatus* status : blockStatusArray_) {
        int row    = status->GetRowNum();
        int column = status->GetColumnNum();

        // colum が違ければ skip
        if (columNum != column || status->GetIsBreak()) {
            continue;
        }

        if (row > baseRowNum && row >= conbinationMax_) {

            bool alreadyExists = std::any_of(result.begin(), result.end(), [&](BlockStatus* block) {
                return block->GetRowNum() == row && block->GetColumnNum() == column;
            });

            if (!alreadyExists) {
                result.push_back(status);
            }
        }
    }

    bool hasAdvantage = std::any_of(result.begin(), result.end(), [](BlockStatus* block) {
        return block->GetBlockType() == BlockType::ADVANTAGE;
    });

    return hasAdvantage;
}


std::vector<BlockStatus*> BlockCombinationStatus::GetRightBlocksForCalucration(const int& baseRowNum, const int& columNum) const {
    std::vector<BlockStatus*> result;
    result.clear();
    for (BlockStatus* status : blockStatusArray_) {
        int row    = status->GetRowNum();
        int column = status->GetColumnNum();

        // colum が違ければ skip
        if (columNum != column) {
            continue;
        }

        if (row < baseRowNum && row >= conbinationMax_) {
            // すでに同じ (row, col) のBlockStatusが result に存在するかをチェック
            bool alreadyExists = std::any_of(result.begin(), result.end(), [&](BlockStatus* block) {
                return block->GetRowNum() == row && block->GetColumnNum() == column;
            });

            if (!alreadyExists) {
                result.push_back(status);
            }
        }
    }

    // BlockType順にソート
    std::sort(result.begin(), result.end(), [](BlockStatus* a, BlockStatus* b) {
        return static_cast<int>(a->GetBlockType()) < static_cast<int>(b->GetBlockType());
    });

    return result;
}

std::vector<BlockStatus*> BlockCombinationStatus::SortBlocksLeftToRight(std::vector<BlockStatus*> blocks) {
    std::vector<BlockStatus*> result = blocks;

    // rowNumが小さい順にソート（右から順）
    std::sort(result.begin(), result.end(), [](BlockStatus* a, BlockStatus* b) {
        return a->GetRowNum() > b->GetRowNum();
    });

    if (!result.empty()) {
        result.back()->SetIsRightEdge(true);
    }

    return result;
}

void BlockCombinationStatus::RemoveMarkedBlocks() {
    blockStatusArray_.erase(
        std::remove_if(blockStatusArray_.begin(), blockStatusArray_.end(),
            [](BlockStatus* status) {
                return status->GetIsBreak();
            }),
        blockStatusArray_.end());
}



// BlockCombinationStatus.cpp
void BlockCombinationStatus::ChangeStatusRightEdge(const int& baseRowNum, const int& columNum) {
    BlockStatus* rightMost = nullptr;

    for (BlockStatus* status : blockStatusArray_) {
        int row    = status->GetRowNum();
        int column = status->GetColumnNum();

        // 条件に合わなければスキップ
        if (column != columNum || status->GetIsBreak()) {
            continue;
        }

        if (row < baseRowNum && row >= conbinationMax_) {
            // 現在の最右（最小row）を更新
            if (rightMost == nullptr || row < rightMost->GetRowNum()) {
                rightMost = status;
            }
        }
    }

    // 全体をもう一度走査してステータス更新
    for (BlockStatus* status : blockStatusArray_) {
        if (status->GetColumnNum() != columNum) {
            continue;
        }

        if (status == rightMost) {
            status->SetFuseMode(FuseMode::END);
        } else {
            if (status->GetFuseMode() == FuseMode::END) {
                status->SetFuseMode(FuseMode::CENTER);
            }
        }
    }
}

void BlockCombinationStatus::ChangeStatusFuse(const int& baseRowNum, const int& columNum) {
    // 対象ブロックを探す
    BlockStatus* targetBlock = nullptr;
    for (BlockStatus* status : blockStatusArray_) {
        if (status->GetColumnNum() == columNum && status->GetRowNum() == baseRowNum && !status->GetIsBreak()) {
            targetBlock = status;
            break;
        }
    }

    // 対象ブロックが見つからない場合は処理終了
    if (!targetBlock) {
        return;
    }

    // 対象ブロックがADVANTAGE以外の場合のみ処理を実行
    if (targetBlock->GetBlockType() != BlockType::ADVANTAGE) {
        bool hasLeftAdvantage = false;

        // 同じ列で、対象ブロックより左側（rowが大きい値）にADVANTAGEブロックがあるかチェック
        for (BlockStatus* status : blockStatusArray_) {
            int row    = status->GetRowNum();
            int column = status->GetColumnNum();

            if (column == columNum && row > baseRowNum && status->GetBlockType() == BlockType::ADVANTAGE && !status->GetIsBreak()) {
                hasLeftAdvantage = true;
                break;
            }
        }

        // 左側にADVANTAGEブロックがある場合、対象ブロックの設定を変更
        if (hasLeftAdvantage) {
            targetBlock->SetFuseMode(FuseMode::CENTER);
            targetBlock->SetIsCreatedFuse(true);
        }
    }
}
void BlockCombinationStatus::ChangeStatusAdvantageStart(const int& baseRowNum, const int& columNum) {
    BlockStatus* leftMostAdvantage = nullptr;

    // 最も左（row が最大）の ADVANTAGE ブロックを探す
    for (BlockStatus* status : blockStatusArray_) {
        int row    = status->GetRowNum();
        int column = status->GetColumnNum();

        if (column != columNum || status->GetIsBreak() || status->GetBlockType() != BlockType::ADVANTAGE) {
            continue;
        }

        if (row < baseRowNum && row >= conbinationMax_) {
            if (leftMostAdvantage == nullptr || row > leftMostAdvantage->GetRowNum()) {
                leftMostAdvantage = status;
            }
        }
    }

    // FuseMode を更新
    for (BlockStatus* status : blockStatusArray_) {
        int row    = status->GetRowNum();
        int column = status->GetColumnNum();

        if (column != columNum || status->GetBlockType() != BlockType::ADVANTAGE) {
            continue;
        }

        if (status == leftMostAdvantage) {
            status->SetFuseMode(FuseMode::START);
        } else if (leftMostAdvantage && row < leftMostAdvantage->GetRowNum()) {
            // leftMost より右（row が小さい）なら CENTER に変更
            status->SetFuseMode(FuseMode::CENTER);
        }
    }
}
