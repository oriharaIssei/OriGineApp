#include "OperateUIStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void OperateUIStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool OperateUIStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();
    isChange = ImGui::DragFloat("scalingEase_.maxTime", &scalingEase_.maxTime);
    isChange = ImGui::DragFloat("scalingEase_.period", &scalingEase_.period);
    isChange = ImGui::DragFloat("scalingEase_.amplitude", &scalingEase_.amplitude);
    isChange = ImGui::DragFloat2("startTextureSize_", startTextureSize_.v);
    isChange = ImGui::DragFloat2("maxTextureSize_", maxTextureSize_.v);
    isChange = ImGui::DragFloat2("resultTextureSize_", resultTextureSize_.v);
    return isChange;
}

void OperateUIStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("scalingEase_.maxTime", scalingEase_.maxTime);
    _writer.Write("scalingEase_.period", scalingEase_.period);
    _writer.Write("scalingEase_.amplitude", scalingEase_.amplitude);

    _writer.Write<2, float>("startTextureSize_", startTextureSize_);
    _writer.Write<2, float>("maxTextureSize_", maxTextureSize_);
    _writer.Write<2, float>("resultTextureSize_", resultTextureSize_);
}

void OperateUIStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("scalingEase_.maxTime", scalingEase_.maxTime);
    _reader.Read("scalingEase_.period", scalingEase_.period);
    _reader.Read("scalingEase_.amplitude", scalingEase_.amplitude);

     _reader.Read<2, float>("startTextureSize_", startTextureSize_);
     _reader.Read<2, float>("maxTextureSize_", maxTextureSize_);
     _reader.Read<2, float>("resultTextureSize_", resultTextureSize_);
}

void OperateUIStatus::Finalize() {}

void OperateUIStatus::ChangeInit(const OperateMode& operateMOde) {
    scalingEase_.time = 0.0f;
    operateMode_      = operateMOde;
    isChange_         = true;
}

void OperateUIStatus::ScalingEaseUpdate(const float& t) {
    if (!isChange_) {
        return;
    }
      
        scalingEase_.time += t;

        /// スケーリングイージング
        resultTextureSize_ = (EaseAmplitudeScale(startTextureSize_, scalingEase_.time, scalingEase_.maxTime,
            scalingEase_.amplitude, scalingEase_.period));

        // 事後処理
        if (scalingEase_.time < scalingEase_.maxTime) {
            return;
        }
        scalingEase_.time = scalingEase_.maxTime;
        resultTextureSize_ = startTextureSize_;
        isChange_         = false;
        
}
