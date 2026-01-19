#include "PlayerInput.h"

/// externals
#ifdef _DEBUG
#include <myGui/MyGui.h>
#endif // _DEBUG

/// math
#include "math/mathEnv.h"
#include "math/Matrix4x4.h"

using namespace OriGine;

void PlayerInput::Initialize(Scene* /*_scene*/, EntityHandle /* _handle */) {
}

void PlayerInput::Edit(Scene* /*_scene*/, EntityHandle /* _handle */, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG
    DragGuiCommand("MaxJumpInputTime" + _parentLabel, maxJumpTime_, 0.01f, 0.01f);
#endif // _DEBUG
}

void PlayerInput::Finalize() {
    // 何もしない
}

Vec3f PlayerInput::CalculateWorldInputDirection(const Quaternion& _cameraRotation) const {
    if (inputDirection_.lengthSq() <= kEpsilon) {
        return Vec3f(0, 0, 0);
    }

    // カメラのYaw軸回転のみ抽出
    float cameraYaw = _cameraRotation.ToYaw();

    // 入力を3Dへ (Zが前)
    Vec3f inputDir3D = {inputDirection_[X], 0.0f, inputDirection_[Y]};
    inputDir3D       = inputDir3D.normalize();

    // カメラのY軸回転を適用
    Vec3f worldDir = inputDir3D * MakeMatrix4x4::RotateY(cameraYaw);
    return worldDir.normalize();
}

void to_json(nlohmann::json& j, const PlayerInput& _input) {
    j["MaxJumpInputTime"] = _input.maxJumpTime_;
}

void from_json(const nlohmann::json& j, PlayerInput& _input) {
    _input.maxJumpTime_ = j["MaxJumpInputTime"].get<float>();
}
