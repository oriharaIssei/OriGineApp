#include "CameraFollowState.h"

/// engine
#include "Engine.h"

/// component
#include "component/camera/CameraController.h"
#include "component/player/state/PlayerState.h"

/// math
#include "MyEasing.h"

using namespace OriGine;

void CameraFollowState::Initialize() {
    lerpTimer_ = 0.0f;
}

void CameraFollowState::Update() {
    CameraController* cc = scene_->GetComponent<CameraController>(cameraEntityHandle_);
    if (!cc) {
        return;
    }

    PlayerState* state = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    if (!state) {
        return;
    }

    float t = 0.f;
    if (state->GetGearLevel() >= kThresholdGearLevel_) {
        float cameraDeltaTime = Engine::GetInstance()->GetDeltaTimer()->GetScaledDeltaTime("Camera");
        lerpTimer_ += cameraDeltaTime;

        t = std::clamp(lerpTimer_ / kLerpTime_, 0.f, 1.f);
    }

    // ギアレベルに応じて目標パラメータを決定
    const CameraStateParams& target =
        (state->GetGearLevel() >= kThresholdGearLevel_) ? cc->dashParams : cc->idleParams;

    cc->currentOffset       = Lerp<3, float>(cc->currentOffset, target.offset, EaseOutCubic(t));
    cc->currentTargetOffset = Lerp<3, float>(cc->currentTargetOffset, target.targetOffset, EaseOutCubic(t));
}

void CameraFollowState::Finalize() {}
