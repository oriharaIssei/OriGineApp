#include "CameraInputSystem.h"

/// stl
#include <algorithm>

/// ecs
#define ENGINE_ECS
#include "EngineInclude.h"
// component
#include "component/cameraController/CameraController.h"
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
    static const float tao             = std::numbers::pi_v<float> * 2.f;
    CameraController* cameraController = getComponent<CameraController>(_entity);

    Vec2f destinationAngleXY = cameraController->getDestinationAngleXY();
    if (cameraController->getFollowTarget()) {
        if (input_->isPadActive()) { /// GamePad
            Vec2f rotateVelocity = input_->getRStickVelocity() * cameraController->getRotateSpeedPadStick();
            // input の x,yをそれぞれの角度に変換
            destinationAngleXY += Vec2f(-rotateVelocity[Y], rotateVelocity[X]);
        } else { /// Mouse
            Vec2f rotateVelocity = input_->getMouseVelocity() * cameraController->getRotateSpeedMouse();
            // input の x,yをそれぞれの角度に変換
            destinationAngleXY += Vec2f(rotateVelocity[Y], rotateVelocity[X]);
        }
        // 角度制限を適用
        destinationAngleXY[X] = std::clamp(destinationAngleXY[X], cameraController->getMinRotateX(), cameraController->getMaxRotateX());
        // Y軸の角度制限を適用
        float forwardAngle    = std::atan2(cameraController->getForward()[X], cameraController->getForward()[Z]);
        destinationAngleXY[Y] = std::clamp(destinationAngleXY[Y], -cameraController->getAngleLimitY() + forwardAngle, cameraController->getAngleLimitY() + forwardAngle);

        cameraController->setDestinationAngleXY(destinationAngleXY);
    }
}
