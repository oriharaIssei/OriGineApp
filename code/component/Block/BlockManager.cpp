#include "BlockManager.h"

#include"component/LevelUPUI/LevelUIParentStatus.h"

/// externals
#include "imgui/imgui.h"
#include <string>

void BlockManager::Initialize([[maybe_unused]] GameEntity* _entity) {
    /// 初期化でパラメータ編集してるから大丈夫、ここ消したら未定義値が出る

    columNumMax_         = 1;
    HPMax_               = 1;
    collisionRadius_     = 1.0f;
    startPositionX_      = 0.0f;
    nextCreatePositionX_ = 0.0f;
    basePosY_            = 0.0f;
    moveTenpo_           = 0.0f;
    deadPositionX_       = 0.0f;
}

bool BlockManager::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::InputInt("pillarHP", &HPMax_);
    isChange |= ImGui::InputInt("columNumMax", &columNumMax_);
    isChange |= ImGui::DragFloat3("blockSize", blockSize_.v);
    isChange |= ImGui::DragFloat3("scalingSize", scalingSize_.v);
    isChange |= ImGui::DragFloat("collisionRadius", &collisionRadius_, 0.01f);
    isChange |= ImGui::DragFloat("startPositionX", &startPositionX_, 0.01f);
    isChange |= ImGui::DragFloat("startPositionZ", &startPositionZ_, 0.01f);
    isChange |= ImGui::DragFloat("nextCreatePositionX", &nextCreatePositionX_, 0.01f);
    isChange |= ImGui::DragFloat("deadPositionX", &deadPositionX_, 0.01f);
    isChange |= ImGui::DragFloat("basePosY", &basePosY_, 0.01f);

    // random create
    ImGui::Text("Random Parameters");
    for (int i = 0; i < randomPar_.size(); ++i) {
        std::string label = "Random[" + std::to_string(i) + "]";
        isChange |= ImGui::InputInt(label.c_str(), &randomPar_[i]);
    }

    ImGui::Text("Costs");
    for (int i = 0; i < costs_.size(); ++i) {
        std::string label = "Cost[" + std::to_string(i) + "]";
        isChange |= ImGui::InputInt(label.c_str(), &costs_[i]);
    }

    ImGui::Text("Generate Interval");
    for (int i = 0; i < generateInterval_.size(); ++i) {
        std::string label = "Interval[" + std::to_string(i) + "]";
        isChange |= ImGui::InputInt(label.c_str(), &generateInterval_[i]);
    }

    ImGui::Text("Score");
    for (int i = 0; i < scoreValue_.size(); ++i) {
        std::string label = "scoreValue[" + std::to_string(i) + "]";
        isChange |= ImGui::DragFloat(label.c_str(), &scoreValue_[i]);
    }

    ImGui::Text("tenpo");
    isChange |= ImGui::DragFloat("moveTenpo", &moveTenpo_, 0.01f);
    isChange |= ImGui::InputInt("movetenpoNum", &moveTenpoNum_);

    for (int i = 0; i < moveTenpos_.size(); ++i) {
        std::string label = "moveTenpos[" + std::to_string(i) + "]";
        ImGui::DragFloat(label.c_str(), &moveTenpos_[i], 0.1f);
    }

    for (int i = 0; i < nextLevelTime_.size(); ++i) {
        std::string label = "nextLevelTime[" + std::to_string(i) + "]";
        ImGui::DragFloat(label.c_str(), &nextLevelTime_[i], 0.1f);
    }

    ImGui::Text("--------------easing---------------");
    ImGui::Text("scalingEase");
    isChange |= ImGui::DragFloat("ScalingmaxTime", &scalingEase_.maxTime, 0.01f);
    /* isChange |= ImGui::DragFloat("Scalingamplitude", &scalingEase_.amplitude, 0.01f);
     isChange |= ImGui::DragFloat("Scalingperiod", &scalingEase_.period, 0.01f);*/
    isChange |= ImGui::DragFloat("ScalingbackRatio", &scalingEase_.backRatio, 0.01f);
    ImGui::Text("scalingEase");
    isChange |= ImGui::DragFloat("MoveScalingmaxTime", &moveScalingEase_.maxTime, 0.01f);
    isChange |= ImGui::DragFloat("MoveScalingamplitude", &moveScalingEase_.amplitude, 0.01f);
    isChange |= ImGui::DragFloat("MoveScalingperiod", &moveScalingEase_.period, 0.01f);
    isChange |= ImGui::DragFloat("MoveScalingbackRatio", &moveScalingEase_.backRatio, 0.01f);
    ImGui::Text("moveEase");
    isChange |= ImGui::DragFloat("moveTimemax", &moveTimemax_, 0.01f);

    return isChange;
}

void BlockManager::Save(BinaryWriter& _writer) {
    _writer.Write("columNumMax", columNumMax_);
    _writer.Write("pillarHP", HPMax_);
    _writer.Write<3, float>("FirstblockSize", blockSize_);
    _writer.Write<3, float>("scalingSize", scalingSize_);
    _writer.Write("collisionRadius", collisionRadius_);
    _writer.Write("startPosition", startPositionX_);
    _writer.Write("nextCreatePositionX", nextCreatePositionX_);
    _writer.Write("basePosY", basePosY_);
    _writer.Write("moveSpeed", moveTenpo_);
    _writer.Write("startPositionZ", startPositionZ_);
    _writer.Write("deadPositionX", deadPositionX_);
    _writer.Write("moveTimemax", moveTimemax_);

    _writer.Write("ScalingmaxTime", scalingEase_.maxTime);
    _writer.Write("Scalingamplitude", scalingEase_.amplitude);
    _writer.Write("Scalingperiod", scalingEase_.period);
    _writer.Write("ScalingbackRatio", scalingEase_.backRatio);

    _writer.Write("MoveScalingmaxTime", moveScalingEase_.maxTime);
    _writer.Write("MoveScalingamplitude", moveScalingEase_.amplitude);
    _writer.Write("MoveScalingperiod", moveScalingEase_.period);
    _writer.Write("MoveScalingbackRatio", moveScalingEase_.backRatio);

    _writer.Write("movetenpoNum", moveTenpoNum_);

    for (int i = 0; i < moveTenpos_.size(); ++i) {
        _writer.Write(("moveSpeeds" + std::to_string(i)).c_str(), moveTenpos_);
    }

    for (int i = 0; i < randomPar_.size(); ++i) {
        _writer.Write(("randomPar_" + std::to_string(i)).c_str(), randomPar_[i]);
    }

    for (int i = 0; i < costs_.size(); ++i) {
        _writer.Write(("costs_" + std::to_string(i)).c_str(), costs_[i]);
    }

    for (int i = 0; i < generateInterval_.size(); ++i) {
        _writer.Write(("generateInterval_" + std::to_string(i)).c_str(), generateInterval_[i]);
    }
    for (int i = 0; i < scoreValue_.size(); ++i) {
        _writer.Write(("scoreValue_" + std::to_string(i)).c_str(), scoreValue_[i]);
    }

      for (int i = 0; i < nextLevelTime_.size(); ++i) {
        _writer.Write(("nextLevelTime" + std::to_string(i)).c_str(), nextLevelTime_);
    }
}

void BlockManager::Load(BinaryReader& _reader) {
    _reader.Read("columNumMax", columNumMax_);
    _reader.Read("pillarHP", HPMax_);
    _reader.Read<3, float>("FirstblockSize", blockSize_);
    _reader.Read<3, float>("scalingSize", scalingSize_);
    _reader.Read("collisionRadius", collisionRadius_);
    _reader.Read("startPosition", startPositionX_);
    _reader.Read("nextCreatePositionX", nextCreatePositionX_);
    _reader.Read("basePosY", basePosY_);
    _reader.Read("moveSpeed", moveTenpo_);
    _reader.Read("startPositionZ", startPositionZ_);
    _reader.Read("deadPositionX", deadPositionX_);
    _reader.Read("moveTimemax", moveTimemax_);

    _reader.Read("ScalingmaxTime", scalingEase_.maxTime);
    _reader.Read("Scalingamplitude", scalingEase_.amplitude);
    _reader.Read("Scalingperiod", scalingEase_.period);
    _reader.Read("ScalingbackRatio", scalingEase_.backRatio);

    _reader.Read("MoveScalingmaxTime", moveScalingEase_.maxTime);
    _reader.Read("MoveScalingamplitude", moveScalingEase_.amplitude);
    _reader.Read("MoveScalingperiod", moveScalingEase_.period);
    _reader.Read("MoveScalingbackRatio", moveScalingEase_.backRatio);

    _reader.Read("movetenpoNum", moveTenpoNum_);

    for (int i = 0; i < moveTenpos_.size(); ++i) {
        _reader.Read(("moveSpeeds" + std::to_string(i)).c_str(), moveTenpos_);
    }

    for (int i = 0; i < randomPar_.size(); ++i) {
        _reader.Read(("randomPar_" + std::to_string(i)).c_str(), randomPar_[i]);
    }

    for (int i = 0; i < costs_.size(); ++i) {
        _reader.Read(("costs_" + std::to_string(i)).c_str(), costs_[i]);
    }

    for (int i = 0; i < generateInterval_.size(); ++i) {
        _reader.Read(("generateInterval_" + std::to_string(i)).c_str(), generateInterval_[i]);
    }

    for (int i = 0; i < scoreValue_.size(); ++i) {
        _reader.Read(("scoreValue_" + std::to_string(i)).c_str(), scoreValue_[i]);
    }

    for (int i = 0; i < nextLevelTime_.size(); ++i) {
        _reader.Read(("nextLevelTime" + std::to_string(i)).c_str(), nextLevelTime_);
    }
}

void BlockManager::Finalize() {}

void BlockManager::CostReset() {
    for (int32_t i = 0; i < currentCosts_.size(); ++i) {
        currentCosts_[i] = 0;
    }
}

void BlockManager::SpeedChangeForTime(float& time, LevelUIParentStatus* levelUI) {

    if (time < nextLevelTime_[currentLevel_]) {
        return;
    }

    currentLevel_++;
    levelUI->SetIsLevelChange(true);
    levelUI->SetNextLevelUV(float(currentLevel_));
    time = 0.0f;
}

void BlockManager::SetMoveTempoForLevel() {
    if (moveTenpos_.size()-1 < currentLevel_) {
        return;
    }

    moveTenpo_ = moveTenpos_[currentLevel_];
 }

void BlockManager::ResetLineCounter(BlockType type) {
    lineCounter_[static_cast<int32_t>(type)] = 0;
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

        //事後処理
        if (moveScalingEase_.time < moveScalingEase_.maxTime) {
            break;
        }
        moveScalingEase_.time = moveScalingEase_.maxTime;
        resultScale_      = blockSize_;
        easeType_         = EaseType::NONE;
        break;
    default:
        break;
    }
}

void BlockManager::ResetScalingEase() {
    scalingEase_.time = 0.0f;
    moveScalingEase_.time = 0.0f;
}
