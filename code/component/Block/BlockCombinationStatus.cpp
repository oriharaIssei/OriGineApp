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



void BlockCombinationStatus::ChangeStatusFuse(const int& baseRowNum, const int& columNum) {
    std::vector<BlockStatus*> result;
    result.reserve(blockStatusArray_.size());

    for (BlockStatus* status : blockStatusArray_) {
        if (!status || status->GetIsBreak())
            continue;

        const int row    = status->GetRowNum();
        const int column = status->GetColumnNum();

        // 列が異なる or row条件を満たさないものはスキップ
        if (column != columNum)
            continue;
        if (row <= baseRowNum)
            continue;
        if (row < conbinationMax_)
            continue;

        // 重複回避（念のため明示）
        auto exists = std::find_if(result.begin(), result.end(), [&](BlockStatus* blk) {
            return blk->GetRowNum() == row && blk->GetColumnNum() == column;
        });
        if (exists == result.end()) {
            result.push_back(status);
        }
    }

    // ADVANTAGEブロックがあるかチェック
    const bool hasAdvantage = std::any_of(result.begin(), result.end(), [](BlockStatus* block) {
        return block->GetBlockType() == BlockType::ADVANTAGE;
    });

    // 対象ブロックを探して状態設定
    for (BlockStatus* status : blockStatusArray_) {
        if (!status || status->GetIsBreak())
            continue;

        if (status->GetColumnNum() == columNum && status->GetRowNum() == baseRowNum) {
            status->SetIsCreateFuse(true);
            status->SetFuseMode(hasAdvantage ? FuseMode::CENTER : FuseMode::NONE);
            break;
        }
    }
}

void BlockCombinationStatus::ChangeStatusAdvantageStart(const int& baseRowNum, const int& columNum) {
    BlockStatus* targetBlock = nullptr;

    for (BlockStatus* status : blockStatusArray_) {
        if (!status || status->GetIsBreak())
            continue;

        if (status->GetColumnNum() == columNum && status->GetRowNum() == baseRowNum) {
            targetBlock = status;
            break;
        }
    }

    if (!targetBlock || targetBlock->GetBlockType() != BlockType::ADVANTAGE) {
        return;
    }

    bool hasLeftAdvantage = false;

    for (BlockStatus* status : blockStatusArray_) {
        if (!status || status->GetIsBreak())
            continue;

        if (status->GetColumnNum() != columNum)
            continue;

        const int row = status->GetRowNum();
        if (row <= baseRowNum)
            continue;
        if (row < conbinationMax_)
            continue;

        if (status->GetBlockType() == BlockType::ADVANTAGE) {
            hasLeftAdvantage = true;
            break;
        }
    }

    targetBlock->SetIsCreateFuse(true);
    targetBlock->SetFuseMode(hasLeftAdvantage ? FuseMode::CENTER : FuseMode::START);
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
