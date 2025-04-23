#include "CameraInputSystem.h"

/// ecs
#define ENGINE_ECS
#include "EngineInclude.h"
// component
#include "component/cameraController/CameraController.h"
// lib
#include "input/Input.h"

void CameraInputSystem::Initialize() {
    input_ = Input::getInstance();
}

void CameraInputSystem::Finalize() {
    input_ = nullptr;
}

void CameraInputSystem::UpdateEntity(GameEntity* _entity) {
    CameraController* cameraController = getComponent<CameraController>(_entity);

    if (cameraController->getFollowTarget()) {
        if (input_->isPadActive()) { /// GamePad
            Vec2f rotateVelocity = input_->getRStickVelocity() * cameraController->getRotateSpeedPadStick();
            // input の x,yをそれぞれの角度に変換
            cameraController->setDestinationAngleXY(cameraController->getDestinationAngleXY() + Vec2f(-rotateVelocity[Y], rotateVelocity[X]));
        } else { /// Mouse
            Vec2f rotateVelocity = input_->getMouseVelocity() * cameraController->getRotateSpeedMouse();
            // input の x,yをそれぞれの角度に変換
            cameraController->setDestinationAngleXY(cameraController->getDestinationAngleXY() + Vec2f(-rotateVelocity[Y], rotateVelocity[X]));
        }

        cameraController->setDestinationAngleXY(Vec2f(
            std::clamp(cameraController->getDestinationAngleXY()[X], cameraController->getMinRotateX(), cameraController->getMaxRotateX()),
            cameraController->getDestinationAngleXY()[Y]));

        cameraController->setInterTarget(Lerp(cameraController->getFollowTarget()->translate, cameraController->getInterTarget(), cameraController->getInterTargetInterpolation()));
    }
}
