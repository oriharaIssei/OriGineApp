#include "LevelUIParentStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void LevelUIParentStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    currentLevelUV_ = 0;
}

bool LevelUIParentStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat3("initPos", initPos_.v);
    isChange |= ImGui::DragFloat3("easePos", easePos_.v);
    isChange |= ImGui::DragFloat2("easeScale", easeScale_.v);
    isChange |= ImGui::DragFloat2("changingEaseScale", changingEaseScale_.v);

     isChange |= ImGui::DragFloat3("moveOffset", moveOffset_.v);

    ImGui::Text("moveEasing");
    isChange |= ImGui::DragFloat("moveEasing.maxTime", &moveEasing_.maxTime,0.01f);
    ImGui::Text("scaleEasing");
    isChange |= ImGui::DragFloat("scaleEasing.maxTime", &scaleEasing_.maxTime, 0.01f);
    isChange |= ImGui::DragFloat("scaleEasing.amplitude", &scaleEasing_.amplitude, 0.01f);
    isChange |= ImGui::DragFloat("scaleEasing.period", &scaleEasing_.period, 0.01f);
    isChange |= ImGui::DragFloat("scaleEasing.backRatio", &scaleEasing_.backRatio, 0.01f);
    ImGui::Text("uvScrollEasing");
    isChange |= ImGui::DragFloat("uvScrollEasing.maxTime", &uvScrollEasing_.maxTime, 0.01f);

      ImGui::Text("waitTime");
    isChange |= ImGui::DragFloat("scrollWaitTime", &scrollWaitTime_, 0.01f);
      isChange |= ImGui::DragFloat("reverseWaitTime", &reverseWaitTime_, 0.01f);

    return isChange;
}

void LevelUIParentStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("moveEasing.maxTime", moveEasing_.maxTime);
    _writer.Write("scaleEasing.maxTime", scaleEasing_.maxTime);
    _writer.Write("scaleEasing.amplitude", scaleEasing_.amplitude);
    _writer.Write("scaleEasing.period", scaleEasing_.period);
    _writer.Write("uvScrollEasing.maxTime", uvScrollEasing_.maxTime);
    _writer.Write<3,float>("easePos", easePos_);
    _writer.Write<2, float>("easeScale", easeScale_);
    _writer.Write("scaleEasing.backRatio", scaleEasing_.backRatio);
    _writer.Write<2, float>("changingEaseScale", changingEaseScale_);
    _writer.Write("scrollWaitTime", scrollWaitTime_);
    _writer.Write("reverseWaitTime", reverseWaitTime_);
    _writer.Write<3, float>("moveOffset", moveOffset_);
    _writer.Write<3, float>("initPos", initPos_);
}

void LevelUIParentStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("moveEasing.maxTime", moveEasing_.maxTime);
    _reader.Read("scaleEasing.maxTime", scaleEasing_.maxTime);
    _reader.Read("scaleEasing.amplitude", scaleEasing_.amplitude);
    _reader.Read("scaleEasing.period", scaleEasing_.period);
    _reader.Read("uvScrollEasing.maxTime", uvScrollEasing_.maxTime);
    _reader.Read<3, float>("easePos", easePos_);
    _reader.Read<2, float>("easeScale", easeScale_);
    _reader.Read("scaleEasing.backRatio", scaleEasing_.backRatio);
    _reader.Read<2, float>("changingEaseScale", changingEaseScale_);
    _reader.Read("scrollWaitTime", scrollWaitTime_);
    _reader.Read("reverseWaitTime", reverseWaitTime_);
    _reader.Read<3, float>("moveOffset", moveOffset_);
    _reader.Read<3, float>("initPos", initPos_);
}

void LevelUIParentStatus::Finalize() {}

void LevelUIParentStatus::MoveAnimation(const float& time) {

    moveEasing_.time += time;

    basePos_   = EaseOutBack(initPos_, easePos_, moveEasing_.time, moveEasing_.maxTime);
    baseScale_         = EaseOutBack(initScale_, easeScale_, moveEasing_.time, moveEasing_.maxTime);
    currentmoveOffset_ = EaseOutBack(Vec3f(0.0f, 0.0f, 0.0f), moveOffset_, moveEasing_.time, moveEasing_.maxTime);

    if (moveEasing_.time < moveEasing_.maxTime) {
        return;
    }

    moveEasing_.time = moveEasing_.maxTime;
    basePos_         = easePos_;
    baseScale_       = easeScale_;
    currentmoveOffset_ = moveOffset_;
    curerntStep_     = AnimationStep::SCROLLWAIT;
}

void LevelUIParentStatus::ScrollAnimation(const float& time) {
    uvScrollEasing_.time += time;

    // 現在のUVを補間計算
    currentLevelUV_ = EaseOutBack(preLevelUV_, nextLevelUV_, uvScrollEasing_.time, uvScrollEasing_.maxTime);

    if (!hasStartedScaling_ && uvScrollEasing_.time >= (uvScrollEasing_.maxTime - 0.2f)) {
        hasStartedScaling_ = true; // フラグを立てて1回だけ起動させる
        curerntStep_       = AnimationStep::SCALING; // 状態変更など必要に応じて
    }

    // UVスクロール完了時の後処理
    if (uvScrollEasing_.time < uvScrollEasing_.maxTime) {
        return;
    }
    
    uvScrollEasing_.time = uvScrollEasing_.maxTime;
    currentLevelUV_      = nextLevelUV_;
    preLevelUV_          = currentLevelUV_;
}


void LevelUIParentStatus::ScalingAnimation(const float& time) {
    scaleEasing_.time += time;
    baseScale_ = EaseAmplitudeScale(easeScale_, scaleEasing_.time, scaleEasing_.maxTime, scaleEasing_.amplitude, scaleEasing_.period);

    if (scaleEasing_.time < scaleEasing_.maxTime) {
        return;
    }

    baseScale_        = easeScale_;
    scaleEasing_.time = scaleEasing_.maxTime;
    curerntStep_      = AnimationStep::REVERSEWAIT;
}

void LevelUIParentStatus::ReverseAnimation(const float& time) {
    moveEasing_.time -= time;

    basePos_           = EaseInBack(initPos_, easePos_, moveEasing_.time, moveEasing_.maxTime);
    baseScale_         = EaseInBack(initScale_, easeScale_, moveEasing_.time, moveEasing_.maxTime);
    currentmoveOffset_ = EaseInBack(Vec3f(0.0f, 0.0f, 0.0f), moveOffset_, moveEasing_.time, moveEasing_.maxTime);


    if (moveEasing_.time > 0.0f) {
        return;
    }

    Reset();
    isLevelChange_ = false;
    curerntStep_ = AnimationStep::NONE;
 }

void LevelUIParentStatus::Reset() {
    moveEasing_.time     = 0.0f;
    uvScrollEasing_.time = 0.0f;
    scaleEasing_.time    = 0.0f;
    basePos_             = initPos_;
    baseScale_           = initScale_;
    currentmoveOffset_   = Vec3f(0.0f, 0.0f, 0.0f);
    hasStartedScaling_   = false;
}

void LevelUIParentStatus::Init() {
    moveEasing_.time     = 0.0f;
    uvScrollEasing_.time = 0.0f;
    scaleEasing_.time    = 0.0f;
    basePos_             = initPos_;
    hasStartedScaling_   = false;
   
}
