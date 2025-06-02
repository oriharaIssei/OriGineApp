#include "ResultUIRankStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void ResultUIRankStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ResultUIRankStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    // ComboDigit（桁数）のUI選択

    isChange |= DragGuiVectorCommand<3, float>("position", position_);

    // 回転量（2種）
    isChange |= ImGui::SliderAngle("rotateValue", &rotateValue_, 0, 1080);

    // スケール
    isChange |= DragGuiVectorCommand("scale", scale_, 0.1f);
    isChange |= DragGuiVectorCommand("easeScale", easeScale_, 0.1f);

    isChange |= ImGui::DragFloat2("textureSize", textureSize_.v, 0.01f);

    ImGui::Text("moveEasing");
    isChange |= DragGuiCommand("scalingEasing_.maxTime", scalingEasing_.maxTime, 0.01f);
    ImGui::Text("scaleEasing");
    isChange |= DragGuiCommand("rotateEasing_.maxTime", rotateEasing_.maxTime, 0.01f);
    ImGui::Text("ScoreValue");
    for (int32_t i = 0; i < rankValue_.size(); ++i) {
        isChange |= DragGuiCommand(("rankValue" + std::to_string(i)).c_str(), rankValue_[i]);
    }

#endif // _DEBUG

    return isChange;
}

void ResultUIRankStatus::Finalize() {}

void ResultUIRankStatus::SetRankForScore(const float& score) {

    if (score < rankValue_[static_cast<int32_t>(RankType::C)]) {
        rankType_ = RankType::C;
        return;
    }

    if (score < rankValue_[static_cast<int32_t>(RankType::B)]) {
        rankType_ = RankType::C;
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
    case RankType::C:
        uvPos_ = 0.0f;
        break;
    case RankType::B:
        uvPos_ = 0.25f;
        break;
    case RankType::A:
        uvPos_ = 0.5f;
        break;
    case RankType::S:
        uvPos_ = 0.75f;
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

void to_json(nlohmann::json& _json, const ResultUIRankStatus& _component) {
    _json["isAlive"]     = _component.isAlive_;
    _json["position"]    = _component.position_;
    _json["rotateValue"] = _component.rotateValue_;
    _json["scale"]       = _component.scale_;
    _json["easeScale"]   = _component.easeScale_;
    _json["textureSize"] = _component.textureSize_;
    for (int32_t i = 0; i < _component.rankValue_.size(); ++i) {
        _json["rankValue" + std::to_string(i)] = _component.rankValue_[i];
    }
    _json["scalingEasing.maxTime"] = _component.scalingEasing_.maxTime;
    _json["rotateEasing.maxTime"]  = _component.rotateEasing_.maxTime;
}
void from_json(const nlohmann::json& _json, ResultUIRankStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    _json.at("position").get_to(_component.position_);
    _json.at("rotateValue").get_to(_component.rotateValue_);
    _json.at("scale").get_to(_component.scale_);
    _json.at("easeScale").get_to(_component.easeScale_);
    _json.at("textureSize").get_to(_component.textureSize_);
    for (int32_t i = 0; i < _component.rankValue_.size(); ++i) {
        if (auto it = _json.find("rankValue" + std::to_string(i)); it != _json.end()) {
            _json.at("rankValue" + std::to_string(i)).get_to(_component.rankValue_[i]);
        }
    }
    _json.at("scalingEasing.maxTime").get_to(_component.scalingEasing_.maxTime);
    _json.at("rotateEasing.maxTime").get_to(_component.rotateEasing_.maxTime);
}
