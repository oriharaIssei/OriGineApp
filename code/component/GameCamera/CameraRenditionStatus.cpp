#include "CameraRenditionStatus.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"
#include <KetaShake.h>
#include <string>

void CameraRenditionStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool CameraRenditionStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiCommand("cameraShakeValue_", cameraShakeValue_, 0.01f);
    isChange |= DragGuiCommand("cameraShakeMaxTime_", cameraShakeMaxTime_, 0.01f);

#endif // _DEBUG

    return isChange;
}

void CameraRenditionStatus::Finalize() {}

void to_json(nlohmann::json& _json, const CameraRenditionStatus& _block) {
    _json["cameraShakeMaxTime_"] = _block.cameraShakeMaxTime_;
    _json["cameraShakeValue_"]   = _block.cameraShakeValue_;
}

void from_json(const nlohmann::json& _json, CameraRenditionStatus& _block) {
    _json.at("cameraShakeMaxTime_").get_to(_block.cameraShakeMaxTime_);
    _json.at("cameraShakeValue_").get_to(_block.cameraShakeValue_);
}

void CameraRenditionStatus::CameraShake(const float& deltaTime) {
    caameraShakeOffset_[X] = ShakeWave<float>(shakeTime_, cameraShakeValue_);
    caameraShakeOffset_[Y] = ShakeWave<float>(shakeTime_, cameraShakeValue_);
    shakeTime_ -= deltaTime;

    if (shakeTime_ > 0.0f) {
        return;
    }

    // リセット
    shakeTime_          = 0.0f;
    caameraShakeOffset_ = {0.0f, 0.0f, 0.0f};
    renditionStep_      = CameraRenditionStep::END;
}

void CameraRenditionStatus::ShakeReset() {
    shakeTime_ = cameraShakeMaxTime_;
    isShaking_ = false;
}
