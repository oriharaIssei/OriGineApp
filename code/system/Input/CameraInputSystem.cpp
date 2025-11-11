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
    GamePadInput* padInput = getScene()->getGamePadInput();
    MouseInput* mouseInput = getScene()->getMouseInput();

    CameraController* cameraController = getComponent<CameraController>(_entity);

    Vec2f destinationAngleXY = cameraController->getDestinationAngleXY();

    if (cameraController->getFollowTarget()) {
        Vec2f rotateVelocity = {0.f, 0.f};
        if (padInput->isActive()) { /// GamePad
            rotateVelocity = padInput->getRStick() * cameraController->getRotateSpeedPadStick();
            // input の x,yをそれぞれの角度に変換
            rotateVelocity = Vec2f(-rotateVelocity[Y], rotateVelocity[X]);
        } else { /// Mouse
            rotateVelocity = mouseInput->getVelocity() * cameraController->getRotateSpeedMouse();
            // input の x,yをそれぞれの角度に変換
            rotateVelocity = Vec2f(rotateVelocity[Y], rotateVelocity[X]);
        }

        destinationAngleXY += rotateVelocity;

        // 角度制限を適用
        destinationAngleXY[X] = std::clamp(destinationAngleXY[X], cameraController->getMinRotateX(), cameraController->getMaxRotateX());

        cameraController->setDestinationAngleXY(destinationAngleXY);
    }
}
