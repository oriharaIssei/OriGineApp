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

    isChange |= ImGui::DragFloat3("position", position_.v);

    // 回転量（2種）
    isChange |= ImGui::SliderAngle("rotateValue", &rotateValue_,0,1080);

    // スケール
    isChange |= ImGui::DragFloat2("scale", scale_.v, 0.1f);
    isChange |= ImGui::DragFloat2("easeScale", easeScale_.v, 0.1f);

    isChange |= ImGui::DragFloat2("textureSize", textureSize_.v, 0.01f);

      ImGui::Text("moveEasing");
    isChange |= ImGui::DragFloat("scalingEasing_.maxTime", &scalingEasing_.maxTime, 0.01f);
    ImGui::Text("scaleEasing");
    isChange |= ImGui::DragFloat("rotateEasing_.maxTime", &rotateEasing_.maxTime, 0.01f);
    ImGui::Text("ScoreValue");
    for (int32_t i = 0; i < rankValue_.size(); ++i) {
        isChange |= ImGui::DragFloat(("rankValue" + std::to_string(i)).c_str(), &rankValue_[i]);
    }
    return isChange;
}

void ResultUIRankStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write<3, float>("position", position_);
    _writer.Write("rotateValue", rotateValue_);
    _writer.Write<2, float>("scale", scale_);
    _writer.Write<2, float>("easeScale", easeScale_);
    _writer.Write<2, float>("textureSize", textureSize_);
    for (int32_t i = 0; i < rankValue_.size(); ++i) {
        _writer.Write(("rankValue" + std::to_string(i)).c_str(), rankValue_[i]);
    }
    _writer.Write("scalingEasing_.maxTime", scalingEasing_.maxTime);
    _writer.Write("rotateEasing_.maxTime", rotateEasing_.maxTime);
}

void ResultUIRankStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read<3, float>("position", position_);
    _reader.Read("rotateValue", rotateValue_);
    _reader.Read<2, float>("scale", scale_);
    _reader.Read<2, float>("easeScale", easeScale_);
    _reader.Read<2, float>("textureSize", textureSize_);
    for (int32_t i = 0; i < rankValue_.size(); ++i) {
        _reader.Read(("rankValue" + std::to_string(i)).c_str(), rankValue_[i]);
    }
    _reader.Read("scalingEasing_.maxTime", scalingEasing_.maxTime);
    _reader.Read("rotateEasing_.maxTime", rotateEasing_.maxTime);
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

void ResultUIRankStatus::ScalingEasing(const float& time) {
    scalingEasing_.time += time;
    scale_ = EaseOutBack(Vec2f(0.0f, 0.0f), easeScale_, scalingEasing_.time, scalingEasing_.maxTime);

    if (scalingEasing_.time < scalingEasing_.maxTime) {
        return;
    }

    scale_              = easeScale_;
    scalingEasing_.time = scalingEasing_.maxTime;
    step_               = RankAnimationStep::END;
}

void ResultUIRankStatus::RotateEasing(const float& time) {
    rotateEasing_.time += time;
    rotate_ = EaseOutBack(0.0f, rotateValue_, rotateEasing_.time, rotateEasing_.maxTime);

    if (rotateEasing_.time < rotateEasing_.maxTime) {
        return;
    }

    rotate_            = 0.0f;
    rotateEasing_.time = rotateEasing_.maxTime;
    /*step_               = RankAnimationStep::END;*/
}

void ResultUIRankStatus::Reset() {
    rotateEasing_.time  = 0.0f;
    scalingEasing_.time = 0.0f;
}
