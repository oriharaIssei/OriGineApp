#include "CameraInputSystem.h"

/// stl
#include <algorithm>

/// engine
#define DELTA_TIME
#include "EngineInclude.h"
#include "scene/Scene.h"
// input
#include "input/GamepadInput.h"
#include "input/MouseInput.h"

// component
#include "component/Camera/CameraController.h"
/// math
#include "math/mathEnv.h"
#include <numbers>

using namespace OriGine;

void CameraInputSystem::Initialize() {}
void CameraInputSystem::Finalize() {}

void CameraInputSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    GamepadInput* padInput = GetScene()->GetGamepadInput();
    MouseInput* mouseInput = GetScene()->GetMouseInput();

    CameraController* cameraController = GetComponent<CameraController>(_handle);

    if (cameraController) {
        InputUpdate(GetMainDeltaTime(), mouseInput, padInput, cameraController);
    }
}

void CameraInputSystem::InputUpdate(float _deltaTime, MouseInput* _mouseInput, GamepadInput* _padInput, CameraController* _cameraController) {

    OriGine::Vec2f destinationAngleXY = _cameraController->GetDestinationAngleXY();

    if (_cameraController->GetFollowTarget()) {
        OriGine::Vec2f rotateVelocity = {0.f, 0.f};
        if (_padInput->IsActive()) { /// GamePad
            rotateVelocity = _padInput->GetRStick() * _cameraController->GetRotateSpeedPadStick();
            // input の x,yをそれぞれの角度に変換
            rotateVelocity = OriGine::Vec2f(-rotateVelocity[Y], rotateVelocity[X]);
        } else { /// Mouse
            rotateVelocity = _mouseInput->GetVelocity() * _cameraController->GetRotateSpeedMouse();
            // input の x,yをそれぞれの角度に変換
            rotateVelocity = OriGine::Vec2f(rotateVelocity[Y], rotateVelocity[X]);
        }
        rotateVelocity *= _deltaTime;

        // 自動注視は入力があったら解除
        bool isInputCamera = rotateVelocity.lengthSq() <= kEpsilon;
        _cameraController->SetIsAutoLookAtPlayer(isInputCamera);

        destinationAngleXY += rotateVelocity;
        destinationAngleXY[Y] = std::fmod(destinationAngleXY[Y], kTao); // Y軸は360度回転可能

        // 角度制限を適用
        destinationAngleXY[X] = std::clamp(destinationAngleXY[X], _cameraController->GetMinRotateX(), _cameraController->GetMaxRotateX());

        _cameraController->SetDestinationAngleXY(destinationAngleXY);
    }
}
