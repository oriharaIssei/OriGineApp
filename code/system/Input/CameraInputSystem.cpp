#include "CameraInputSystem.h"

/// stl
#include <algorithm>

/// ecs
#define ENGINE_ECS
#define DELTA_TIME
#include "EngineInclude.h"
// component
#include "component/Camera/CameraController.h"
// lib
#include "input/Input.h"

/// math
#include <numbers>

void CameraInputSystem::Initialize() {
}

void CameraInputSystem::Finalize() {
}

void CameraInputSystem::UpdateEntity(Entity* _entity) {
    Input* input                       = Input::getInstance();
    CameraController* cameraController = getComponent<CameraController>(_entity);

    Vec2f destinationAngleXY = cameraController->getDestinationAngleXY();

    if (cameraController->getFollowTarget()) {
        Vec2f rotateVelocity = {0.f, 0.f};
        if (input->isPadActive()) { /// GamePad
            rotateVelocity = input->getRStickVelocity() * cameraController->getRotateSpeedPadStick();
            // input の x,yをそれぞれの角度に変換
            rotateVelocity = Vec2f(-rotateVelocity[Y], rotateVelocity[X]);
        } else { /// Mouse
            rotateVelocity = input->getMouseVelocity() * cameraController->getRotateSpeedMouse();
            // input の x,yをそれぞれの角度に変換
            rotateVelocity = Vec2f(rotateVelocity[Y], rotateVelocity[X]);
        }

        destinationAngleXY += rotateVelocity;

        // 角度制限を適用
        destinationAngleXY[X] = std::clamp(destinationAngleXY[X], cameraController->getMinRotateX(), cameraController->getMaxRotateX());
        // Y軸の角度制限を適用
        /*destinationAngleXY[Y] = std::clamp(destinationAngleXY[Y], -cameraController->getAngleLimitY() , cameraController->getAngleLimitY() );*/

        cameraController->setDestinationAngleXY(destinationAngleXY);
    }
}
