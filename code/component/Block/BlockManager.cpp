#include "BlockManager.h"

#include "component/LevelUPUI/LevelUIParentStatus.h"

#include <string>
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void BlockManager::Initialize([[maybe_unused]] GameEntity* _entity) {
    /// 初期化でパラメータ編集してるから大丈夫、ここ消したら未定義値が出る

    /* columNumMax_         = 1;
     HPMax_               = 1;
     collisionRadius_     = 1.0f;
     startPositionX_      = 0.0f;
     nextCreatePositionX_ = 0.0f;
     basePosY_            = 0.0f;
     moveTenpo_           = 0.0f;
     deadPositionX_       = 0.0f;*/

    currentLevel_ = 0;
}

bool BlockManager::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    isChange |= InputGuiCommand<int>("pillarHP", HPMax_);
    isChange |= InputGuiCommand<int>("columnMax", columnMax_);
    isChange |= InputGuiCommand<int>("rowMax", rowMax_);
    isChange |= DragGuiVectorCommand<3, float>("blockSize", blockSize_);
    isChange |= DragGuiVectorCommand<3, float>("scalingSize", scalingSize_);
    isChange |= DragGuiCommand("collisionRadius", collisionRadius_, 0.01f);
    isChange |= DragGuiCommand("startPositionX", startPositionX_, 0.01f);
    isChange |= DragGuiCommand("startPositionZ", startPositionZ_, 0.01f);
    isChange |= DragGuiCommand("nextCreatePositionX", nextCreatePositionX_, 0.01f);
    isChange |= DragGuiCommand("deadPositionX", deadPositionX_, 0.01f);
    isChange |= DragGuiCommand("basePosY", basePosY_, 0.01f);

    ImGui::Text("Random Parameters");
    for (int i = 0; i < static_cast<int>(BlockType::COUNT); ++i) {
        std::string label = std::string("Random[") + ToStringByBlockType(static_cast<BlockType>(i)) + "]";
        isChange |= InputGuiCommand<int>(label.c_str(), randomPar_[i]);
    }

    ImGui::Text("Costs");
    for (int i = 0; i < static_cast<int>(BlockType::COUNT); ++i) {
        std::string label = std::string("Cost[") + ToStringByBlockType(static_cast<BlockType>(i)) + "]";
        isChange |= InputGuiCommand<int>(label.c_str(), costs_[i]);
    }

    ImGui::Text("Generate Interval");
    for (int i = 0; i < static_cast<int>(BlockType::COUNT); ++i) {
        std::string label = std::string("Interval[") + ToStringByBlockType(static_cast<BlockType>(i)) + "]";
        isChange |= InputGuiCommand<int>(label.c_str(), generateInterval_[i]);
    }

    ImGui::Text("randomParRightofAdvance");
    for (int i = 0; i < static_cast<int>(BlockType::COUNT); ++i) {
        std::string label = std::string("randomParRightofAdvance[") + ToStringByBlockType(static_cast<BlockType>(i)) + "]";
        isChange |= InputGuiCommand<int>(label.c_str(), randomParRightofAdvance[i]);
    }

    ImGui::Text("randomPar InOutOfCost");
    for (int i = 0; i < static_cast<int>(BlockType::COUNT); ++i) {
        std::string label = std::string("randomParInOutOfCost[") + ToStringByBlockType(static_cast<BlockType>(i)) + "]";
        isChange |= InputGuiCommand<int>(label.c_str(), randomParConstant_[i]);
    }

    ImGui::Text("randomParUPValue_");
    for (int i = 0; i < static_cast<int>(BlockType::COUNT); ++i) {
        std::string label = std::string("randomParUPValue_[") + ToStringByBlockType(static_cast<BlockType>(i)) + "]";
        isChange |= InputGuiCommand<int>(label.c_str(), randomParUPValue_[i]);
    }

    ImGui::Text("Score");
    for (int i = 0; i < scoreValue_.size(); ++i) {
        std::string label = "scoreValue[" + std::to_string(i) + "]";
        isChange |= DragGuiCommand(label.c_str(), scoreValue_[i]);
    }

    ImGui::Text("tenpo");
    isChange |= DragGuiCommand("moveTenpo", moveTenpo_, 0.01f);
    isChange |= InputGuiCommand<int>("movetenpoNum", moveTenpoNum_);

    for (int i = 0; i < moveTenpos_.size(); ++i) {
        std::string label = "moveTenpos[" + std::to_string(i) + "]";
        DragGuiCommand(label.c_str(), moveTenpos_[i], 0.1f);
    }

    for (int i = 0; i < nextLevelTime_.size(); ++i) {
        std::string label = "nextLevelTime[" + std::to_string(i) + "]";
        DragGuiCommand(label.c_str(), nextLevelTime_[i], 0.1f);
    }

    ImGui::Text("--------------easing---------------");
    ImGui::Text("scalingEase");
    isChange |= DragGuiCommand("ScalingmaxTime", scalingEase_.maxTime, 0.01f);
    /* isChange |= DragGuiCommand("Scalingamplitude", &scalingEase_.amplitude, 0.01f);
     isChange |= DragGuiCommand("Scalingperiod", &scalingEase_.period, 0.01f);*/
    isChange |= DragGuiCommand("ScalingbackRatio", scalingEase_.backRatio, 0.01f);
    ImGui::Text("scalingEase");
    isChange |= DragGuiCommand("MoveScalingmaxTime", moveScalingEase_.maxTime, 0.01f);
    isChange |= DragGuiCommand("MoveScalingamplitude", moveScalingEase_.amplitude, 0.01f);
    isChange |= DragGuiCommand("MoveScalingperiod", moveScalingEase_.period, 0.01f);
    isChange |= DragGuiCommand("MoveScalingbackRatio", moveScalingEase_.backRatio, 0.01f);
    ImGui::Text("moveEase");
    isChange |= DragGuiCommand("moveTimemax", moveTimemax_, 0.01f);

#endif // _DEBUG

    return isChange;
}

void BlockManager::Finalize() {}

void BlockManager::CostReset() {
    for (int32_t i = 0; i < currentCosts_.size(); ++i) {
        currentCosts_[i] = 0;
    }
}

void BlockManager::SpeedChangeForTime(float& time, LevelUIParentStatus* levelUI) {

    if (moveTenpos_.size() - 1 < currentLevel_) {
        return;
    }

    if (time < nextLevelTime_[currentLevel_]) {
        return;
    }

    currentLevel_++;
    levelUI->SetIsLevelChange(true);
    levelUI->SetNextLevelUV(float(currentLevel_));
    time = 0.0f;
}

void BlockManager::SetMoveTempoForLevel() {
    if (moveTenpos_.size() - 1 < currentLevel_) {
        return;
    }

    moveTenpo_ = moveTenpos_[currentLevel_];
}

void BlockManager::ResetLineCounter(BlockType type) {
    type;
    /* lineCounter_= 0;*/
}

int32_t BlockManager::GetLineCounter(BlockType type) const {
    return lineCounter_[static_cast<int32_t>(type)] + randomParRightofAdvance[static_cast<int32_t>(type)];
}

void BlockManager::LineIncrement() {
    SetIncrementLineCounter(BlockType::NORMAL);
    SetIncrementLineCounter(BlockType::ADVANTAGE);
    SetIncrementLineCounter(BlockType::SKULL);
}

void BlockManager::ScalingEaseUpdate(const float& t) {
    switch (easeType_) {
    case EaseType::NONE:
        return;
        break;
        ///------------------------------------------------------------------------------------------
        /// Scaling
        ///-------------------------------------------------------------------------------------------
    case EaseType::SCALING:

        scalingEase_.time += t;

        /// スケーリングイージング
        resultScale_ = (Back::InCubicZero(blockSize_, scalingSize_, scalingEase_.time, scalingEase_.maxTime,
            scalingEase_.backRatio));

        // 事後処理
        if (scalingEase_.time < scalingEase_.maxTime) {
            break;
        }
        scalingEase_.time = scalingEase_.maxTime;
        resultScale_      = blockSize_;
        easeType_         = EaseType::NONE;

        break;
        ///------------------------------------------------------------------------------------------
        /// MoveScaling
        ///-------------------------------------------------------------------------------------------
    case EaseType::MOVESCALING:
        moveScalingEase_.time += t;

        /// スケーリングイージング
        resultScale_ = EaseAmplitudeScale(blockSize_, moveScalingEase_.time, moveScalingEase_.maxTime,
            moveScalingEase_.amplitude, moveScalingEase_.period);

        // 事後処理
        if (moveScalingEase_.time < moveScalingEase_.maxTime) {
            break;
        }
        moveScalingEase_.time = moveScalingEase_.maxTime;
        resultScale_          = blockSize_;
        easeType_             = EaseType::NONE;
        break;
    default:
        break;
    }
}

void BlockManager::ResetScalingEase() {
    scalingEase_.time     = 0.0f;
    moveScalingEase_.time = 0.0f;
}

void to_json(nlohmann::json& _json, const BlockManager& _blockManager) {
    _json["columNumMax"]          = _blockManager.columnMax_;
    _json["pillarHP"]             = _blockManager.HPMax_;
    _json["FirstblockSize"]       = _blockManager.blockSize_;
    _json["scalingSize"]          = _blockManager.scalingSize_;
    _json["collisionRadius"]      = _blockManager.collisionRadius_;
    _json["startPosition"]        = _blockManager.startPositionX_;
    _json["nextCreatePositionX"]  = _blockManager.nextCreatePositionX_;
    _json["basePosY"]             = _blockManager.basePosY_;
    _json["moveSpeed"]            = _blockManager.moveTenpo_;
    _json["startPositionZ"]       = _blockManager.startPositionZ_;
    _json["deadPositionX"]        = _blockManager.deadPositionX_;
    _json["moveTimemax"]          = _blockManager.moveTimemax_;
    _json["ScalingmaxTime"]       = _blockManager.scalingEase_.maxTime;
    _json["Scalingamplitude"]     = _blockManager.scalingEase_.amplitude;
    _json["Scalingperiod"]        = _blockManager.scalingEase_.period;
    _json["ScalingbackRatio"]     = _blockManager.scalingEase_.backRatio;
    _json["MoveScalingmaxTime"]   = _blockManager.moveScalingEase_.maxTime;
    _json["MoveScalingamplitude"] = _blockManager.moveScalingEase_.amplitude;
    _json["MoveScalingperiod"]    = _blockManager.moveScalingEase_.period;
    _json["MoveScalingbackRatio"] = _blockManager.moveScalingEase_.backRatio;
    _json["movetenpoNum"]         = _blockManager.moveTenpoNum_;
    _json["moveSpeeds"]           = _blockManager.moveTenpos_;
    _json["randomPar"]            = _blockManager.randomPar_;
    _json["costs"]                = _blockManager.costs_;

    _json["generateInterval"] = _blockManager.generateInterval_;
    _json["scoreValue"]       = _blockManager.scoreValue_;
    _json["nextLevelTime"]    = _blockManager.nextLevelTime_;
    _json["rowNumMax"]        = _blockManager.rowMax_;

    _json["lineOffset"]           = _blockManager.randomParRightofAdvance;
    _json["randomParInOutOfCost"] = _blockManager.randomParConstant_;

    _json["randomParUPValue"] = _blockManager.randomParUPValue_;
}

void from_json(const nlohmann::json& _json, BlockManager& _blockManager) {
    _json.at("columNumMax").get_to(_blockManager.columnMax_);
    _json.at("pillarHP").get_to(_blockManager.HPMax_);
    _json.at("FirstblockSize").get_to(_blockManager.blockSize_);
    _json.at("scalingSize").get_to(_blockManager.scalingSize_);
    _json.at("collisionRadius").get_to(_blockManager.collisionRadius_);
    _json.at("startPosition").get_to(_blockManager.startPositionX_);
    _json.at("nextCreatePositionX").get_to(_blockManager.nextCreatePositionX_);
    _json.at("basePosY").get_to(_blockManager.basePosY_);
    _json.at("moveSpeed").get_to(_blockManager.moveTenpo_);
    _json.at("startPositionZ").get_to(_blockManager.startPositionZ_);
    _json.at("deadPositionX").get_to(_blockManager.deadPositionX_);
    _json.at("moveTimemax").get_to(_blockManager.moveTimemax_);
    _json.at("ScalingmaxTime").get_to(_blockManager.scalingEase_.maxTime);
    _json.at("Scalingamplitude").get_to(_blockManager.scalingEase_.amplitude);
    _json.at("Scalingperiod").get_to(_blockManager.scalingEase_.period);
    _json.at("ScalingbackRatio").get_to(_blockManager.scalingEase_.backRatio);
    _json.at("MoveScalingmaxTime").get_to(_blockManager.moveScalingEase_.maxTime);
    _json.at("MoveScalingamplitude").get_to(_blockManager.moveScalingEase_.amplitude);
    _json.at("MoveScalingperiod").get_to(_blockManager.moveScalingEase_.period);
    _json.at("MoveScalingbackRatio").get_to(_blockManager.moveScalingEase_.backRatio);
    _json.at("movetenpoNum").get_to(_blockManager.moveTenpoNum_);
    _json.at("moveSpeeds").get_to(_blockManager.moveTenpos_);
    _json.at("randomPar").get_to(_blockManager.randomPar_);
    _json.at("costs").get_to(_blockManager.costs_);
    _json.at("generateInterval").get_to(_blockManager.generateInterval_);
    _json.at("scoreValue").get_to(_blockManager.scoreValue_);
    _json.at("nextLevelTime").get_to(_blockManager.nextLevelTime_);

    if (auto it = _json.find("rowNumMax"); it != _json.end()) {
        _json.at("rowNumMax").get_to(_blockManager.rowMax_);
    }

    if (auto it = _json.find("lineOffset"); it != _json.end()) {
        _json.at("lineOffset").get_to(_blockManager.randomParRightofAdvance);
    }

    if (auto it = _json.find("randomParInOutOfCost"); it != _json.end()) {
        _json.at("randomParInOutOfCost").get_to(_blockManager.randomParConstant_);
    }

    if (auto it = _json.find("randomParUPValue"); it != _json.end()) {
        _json.at("randomParUPValue").get_to(_blockManager.randomParUPValue_);
    }
}

const char* BlockManager::ToStringByBlockType(BlockType type) {
    switch (type) {
    case BlockType::NORMAL:
        return "NORMAL";
    case BlockType::ADVANTAGE:
        return "ADVANTAGE";
    case BlockType::SKULL:
        return "SKULL";
    default:
        return "UNKNOWN";
    }
}
