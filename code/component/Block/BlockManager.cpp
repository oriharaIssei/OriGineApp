#include "BlockManager.h"
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
    moveSpeed_           = 0.0f;
    deadPositionX_       = 0.0f;
    
}

bool BlockManager::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::InputInt("pillarHP", &HPMax_);
    isChange |= ImGui::InputInt("columNumMax", &columNumMax_);
    isChange |= ImGui::DragFloat2("blockSize", blockSize_.v);
    isChange |= ImGui::DragFloat("collisionRadius", &collisionRadius_, 0.01f);
    isChange |= ImGui::DragFloat("startPositionX", &startPositionX_, 0.01f);
    isChange |= ImGui::DragFloat("startPositionZ", &startPositionZ_, 0.01f);
    isChange |= ImGui::DragFloat("nextCreatePositionX", &nextCreatePositionX_, 0.01f);
    isChange |= ImGui::DragFloat("deadPositionX", &deadPositionX_, 0.01f);
    isChange |= ImGui::DragFloat("basePosY", &basePosY_, 0.01f);
    isChange |= ImGui::DragFloat("moveSpeedStart", &moveSpeed_, 0.01f);

    for (int i = 0; i < moveSpeeds_.size(); ++i) {
        std::string label = "moveSpeed[" + std::to_string(i) + "]";
        ImGui::DragFloat(label.c_str(), &moveSpeeds_[i], 0.1f);
    }

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


    return isChange;
}

void BlockManager::Save(BinaryWriter& _writer) {
    _writer.Write("columNumMax", columNumMax_);
    _writer.Write("pillarHP", HPMax_);
    _writer.Write<2, float>("blockSize", blockSize_);
    _writer.Write("collisionRadius", collisionRadius_);
    _writer.Write("startPosition", startPositionX_);
    _writer.Write("nextCreatePositionX", nextCreatePositionX_);
    _writer.Write("basePosY", basePosY_);
    _writer.Write("moveSpeed", moveSpeed_);
    _writer.Write("startPositionZ", startPositionZ_);
    _writer.Write("deadPositionX", deadPositionX_);
    for (int i = 0; i < moveSpeeds_.size(); ++i) {
        _writer.Write(("moveSpeeds" + std::to_string(i)).c_str(), moveSpeeds_);
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
   
}

void BlockManager::Load(BinaryReader& _reader) {
    _reader.Read("columNumMax", columNumMax_);
    _reader.Read("pillarHP", HPMax_);
    _reader.Read<2, float>("blockSize", blockSize_);
    _reader.Read("collisionRadius", collisionRadius_);
    _reader.Read("startPosition", startPositionX_);
    _reader.Read("nextCreatePositionX", nextCreatePositionX_);
    _reader.Read("basePosY", basePosY_);
    _reader.Read("moveSpeed", moveSpeed_);
    _reader.Read("startPositionZ", startPositionZ_);
    _reader.Read("deadPositionX", deadPositionX_);
    for (int i = 0; i < moveSpeeds_.size(); ++i) {
        _reader.Read(("moveSpeeds" + std::to_string(i)).c_str(), moveSpeeds_);
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
   
}

void BlockManager::Finalize() {}

void BlockManager::CostReset() {
    for (int32_t i = 0; i < currentCosts_.size(); ++i) {
        currentCosts_[i] = 0;
    }
}

void BlockManager::SpeedChangeForTime(const float& time) {
    // 各速度の切り替え間隔（例：10秒ごとに切り替え）
    const float interval = 20.0f;

    // 現在のインデックスを計算
    size_t index = static_cast<size_t>(time / interval);

    // 範囲外対策（最大インデックスに固定）
    index = std::min(index, moveSpeeds_.size() - 1);

    // 現在の速度を設定
    moveSpeed_ = moveSpeeds_[index];
}

void BlockManager::ResetLineCounter(BlockType type) {
    lineCounter_[static_cast<int32_t>(type)] = 0;
}
