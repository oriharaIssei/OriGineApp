#include "ResultUIParentStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void ResultUIParentStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ResultUIParentStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat3("initPos", initPos_.v);
    isChange |= ImGui::DragFloat3("easePos", easePos_.v);
    isChange |= ImGui::DragFloat2("easeScale", scoreEaseScale_.v);
    isChange |= ImGui::DragFloat("scoreUpSpeed", &scoreUpSpeed_, 0.01f);

    ImGui::Text("moveEasing");
    isChange |= ImGui::DragFloat("moveEasing.maxTime", &moveEasing_.maxTime, 0.01f);
    ImGui::Text("scaleEasing");
    isChange |= ImGui::DragFloat("scaleEasing.maxTime", &scaleEasing_.maxTime, 0.01f);
    isChange |= ImGui::DragFloat("scaleEasing.amplitude", &scaleEasing_.amplitude, 0.01f);
    isChange |= ImGui::DragFloat("scaleEasing.period", &scaleEasing_.period, 0.01f);
    isChange |= ImGui::DragFloat("scaleEasing.backRatio", &scaleEasing_.backRatio, 0.01f);
    ImGui::Text("AlphaEasing");
    isChange |= ImGui::DragFloat("alphaEasing.maxTime", &alphaEasing_.maxTime, 0.01f);
    ImGui::Text("waitTime");
    isChange |= ImGui::DragFloat("scoreWaitTime", &scoreWaitTime_, 0.01f);

    return isChange;
}

void ResultUIParentStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("moveEasing.maxTime", moveEasing_.maxTime);
    _writer.Write("scaleEasing.maxTime", scaleEasing_.maxTime);
    _writer.Write("scaleEasing.amplitude", scaleEasing_.amplitude);
    _writer.Write("scaleEasing.period", scaleEasing_.period);
    _writer.Write<3, float>("easePos", easePos_);
    _writer.Write<2, float>("easeScale", scoreEaseScale_);
    _writer.Write("scaleEasing.backRatio", scaleEasing_.backRatio);
    _writer.Write("scoreWaitTime", scoreWaitTime_);
    _writer.Write<3, float>("initPos", initPos_);
    _writer.Write("alphaEasing.maxTime", alphaEasing_.maxTime);
    _writer.Write("scoreUpSpeed", scoreUpSpeed_);
}

void ResultUIParentStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("moveEasing.maxTime", moveEasing_.maxTime);
    _reader.Read("scaleEasing.maxTime", scaleEasing_.maxTime);
    _reader.Read("scaleEasing.amplitude", scaleEasing_.amplitude);
    _reader.Read("scaleEasing.period", scaleEasing_.period);
    _reader.Read<3, float>("easePos", easePos_);
    _reader.Read<2, float>("easeScale", scoreEaseScale_);
    _reader.Read("scaleEasing.backRatio", scaleEasing_.backRatio);
    _reader.Read("scoreWaitTime", scoreWaitTime_);
    _reader.Read<3, float>("initPos", initPos_);
    _reader.Read("alphaEasing.maxTime", alphaEasing_.maxTime);
    _reader.Read("scoreUpSpeed", scoreUpSpeed_);
}

void ResultUIParentStatus::Finalize() {}

void ResultUIParentStatus::MoveAnimation(const float& time) {

    moveEasing_.time += time;

    basePos_    = EaseOutBack(initPos_, easePos_, moveEasing_.time, moveEasing_.maxTime);
    scoreScale_ = EaseOutBack(scoreInitScale_, scoreEaseScale_, moveEasing_.time, moveEasing_.maxTime);

    if (moveEasing_.time < moveEasing_.maxTime) {
        return;
    }

    moveEasing_.time = moveEasing_.maxTime;
    basePos_         = easePos_;
    scoreScale_      = scoreEaseScale_;
    curerntStep_     = ResultStep::SCOREUPWAIT;
}

void ResultUIParentStatus::AlphaAnimation(const float& time) {
    alphaEasing_.time += time;

    // 現在のUVを補間計算
    alpha_ = EaseOutBack(0.0f, 1.0f, alphaEasing_.time, alphaEasing_.maxTime);

    // UVスクロール完了時の後処理
    if (alphaEasing_.time < alphaEasing_.maxTime) {
        return;
    }

    alphaEasing_.time = alphaEasing_.maxTime;
    alpha_            = 1.0f;
}

void ResultUIParentStatus::ScoreScalingAnimation(const float& time) {
    scaleEasing_.time += time;
    scoreScale_ = EaseAmplitudeScale(scoreEaseScale_, scaleEasing_.time, scaleEasing_.maxTime, scaleEasing_.amplitude, scaleEasing_.period);

    if (scaleEasing_.time < scaleEasing_.maxTime) {
        return;
    }

    scoreScale_       = scoreEaseScale_;
    scaleEasing_.time = scaleEasing_.maxTime;
    curerntStep_      = ResultStep::END;
}

void ResultUIParentStatus::ScoreUP(const float& time) {
    currentScore_ += scoreUpSpeed_ * time;

    if (currentScore_ < resultScore_) {
        return;
    }

    currentScore_ = resultScore_;
    curerntStep_  = ResultStep::SCOREUPSCALING;

}

    void ResultUIParentStatus::Reset() {
        moveEasing_.time   = 0.0f;
        scaleEasing_.time  = 0.0f;
        alphaEasing_.time  = 0.0f;
        basePos_           = initPos_;
        scoreScale_        = scoreInitScale_;
        hasStartedScaling_ = false;
    }
