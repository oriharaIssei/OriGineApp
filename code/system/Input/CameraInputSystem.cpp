#include "CameraInputSystem.h"

/// stl
#include <algorithm>

/// engine
// input
#include "input/GamePadInput.h"
#include "input/KeyboardInput.h"
#include "input/MouseInput.h"
/// ecs
#define ENGINE_ECS
#define DELTA_TIME
#include "EngineInclude.h"
// component
#include "component/Camera/CameraController.h"
/// math
#include <numbers>

void CameraInputSystem::Initialize() {}
void CameraInputSystem::Finalize() {}

void CameraInputSystem::UpdateEntity(Entity* _entity) {
    GamePadInput* padInput = GetScene()->GetGamePadInput();
    MouseInput* mouseInput = GetScene()->GetMouseInput();

    CameraController* cameraController = GetComponent<CameraController>(_entity);

    Vec2f destinationAngleXY = cameraController->GetDestinationAngleXY();

    if (cameraController->GetFollowTarget()) {
        Vec2f rotateVelocity = {0.f, 0.f};
        if (padInput->IsActive()) { /// GamePad
            rotateVelocity = padInput->GetRStick() * cameraController->GetRotateSpeedPadStick();
            // input の x,yをそれぞれの角度に変換
            rotateVelocity = Vec2f(-rotateVelocity[Y], rotateVelocity[X]);
        } else { /// Mouse
            rotateVelocity = mouseInput->GetVelocity() * cameraController->GetRotateSpeedMouse();
            // input の x,yをそれぞれの角度に変換
            rotateVelocity = Vec2f(rotateVelocity[Y], rotateVelocity[X]);
        }

        destinationAngleXY += rotateVelocity;

        // 角度制限を適用
        destinationAngleXY[X] = std::clamp(destinationAngleXY[X], cameraController->GetMinRotateX(), cameraController->GetMaxRotateX());

        cameraController->SetDestinationAngleXY(destinationAngleXY);
    }
}
