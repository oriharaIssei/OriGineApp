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
    input_ = Input::getInstance();
}

void CameraInputSystem::Finalize() {
    input_ = nullptr;
}

void CameraInputSystem::UpdateEntity(GameEntity* _entity) {
    CameraController* cameraController = getComponent<CameraController>(_entity);

    Vec2f destinationAngleXY = cameraController->getDestinationAngleXY();

    if (cameraController->getFollowTarget()) {
        Vec2f rotateVelocity = {0.f, 0.f};
        if (input_->isPadActive()) { /// GamePad
            rotateVelocity = input_->getRStickVelocity() * cameraController->getRotateSpeedPadStick();
            // input の x,yをそれぞれの角度に変換
            rotateVelocity = Vec2f(-rotateVelocity[Y], rotateVelocity[X]);
        } else { /// Mouse
            rotateVelocity = input_->getMouseVelocity() * cameraController->getRotateSpeedMouse();
            // input の x,yをそれぞれの角度に変換
            rotateVelocity = Vec2f(rotateVelocity[Y], rotateVelocity[X]);
        }

        // 角度制限を適用
        destinationAngleXY[X] = std::clamp(destinationAngleXY[X], cameraController->getMinRotateX(), cameraController->getMaxRotateX());
        // Y軸の角度制限を適用
        /*destinationAngleXY[Y] = std::clamp(destinationAngleXY[Y], -cameraController->getAngleLimitY() , cameraController->getAngleLimitY() );*/

        cameraController->setDestinationAngleXY(destinationAngleXY);
    }
}
