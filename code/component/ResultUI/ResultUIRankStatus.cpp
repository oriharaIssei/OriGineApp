#include "ResultUIRankStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void ResultUIRankStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ResultUIRankStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    // ComboDigit（桁数）のUI選択

    isChange |= ImGui::DragFloat3("offsetPos", offsetPos_.v);

    for (int32_t i = 0; i < rankValue_.size(); ++i) {
        isChange |= ImGui::DragFloat(("rankValue" + std::to_string(i)).c_str(), &rankValue_[i]);
    }
    return isChange;
}

void ResultUIRankStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write<3, float>("offsetPos", offsetPos_);
}

void ResultUIRankStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read<3, float>("offsetPos", offsetPos_);
}

void ResultUIRankStatus::Finalize() {}

void ResultUIRankStatus::SetRankForScore(const float& score) {

    if (score < rankValue_[static_cast<int32_t>(RankType::B)]) {
        rankType_ = RankType::B;
        return;
    }

    if (score < rankValue_[static_cast<int32_t>(RankType::A)]) {
        rankType_ = RankType::B;
        return;
    }

    if (score < rankValue_[static_cast<int32_t>(RankType::S)]) {
        rankType_ = RankType::A;
        return;
    }

    rankType_ = RankType::S;
}

float ResultUIRankStatus::GetUVPos() {

    switch (rankType_) {
    case RankType::B:
        uvPos_ = 0.0f;
        break;
    case RankType::A:
        uvPos_ = 0.1f;
        break;
    case RankType::S:
        uvPos_ = 0.2f;
        break;
    default:
        uvPos_ = 0;
        break;
    }
    return uvPos_;
}
