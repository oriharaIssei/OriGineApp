#include "CameraInputSystem.h"

/// stl
#include <algorithm>

/// ecs
#define ENGINE_ECS
#define DELTA_TIME
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
    static const float tao = std::numbers::pi_v<float> * 2.f;
    const float deltaTime  = getMainDeltaTime();

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

        // 入力があるならそのまま ないなら 正面を向くように補正
        if (rotateVelocity.lengthSq() >= 0.0001f) {
            destinationAngleXY += rotateVelocity;
        } else {
            // X軸を0に近づける
            if (std::abs(destinationAngleXY[X]) < 0.001f) {
                destinationAngleXY[X] = 0.f;
            } else {
                bool signedB  = (destinationAngleXY[X] > 0.f);
                float signedF = signedB ? 1.f : -1.f;
                destinationAngleXY[X] -= signedF * (cameraController->getFixForForwardSpeed() * deltaTime);

                // 0を超えたら0にする
                if ((signedB && destinationAngleXY[X] < 0.f) || (!signedB && destinationAngleXY[X] > 0.f)) {
                    destinationAngleXY[X] = 0.f;
                }
            }

            // Y軸を正面に近づける
            float forwardAngle = std::atan2(cameraController->getForward()[X], cameraController->getForward()[Z]);
            float deltaY       = destinationAngleXY[Y] - forwardAngle;
            if (std::abs(deltaY) < 0.001f) {
                destinationAngleXY[Y] = forwardAngle;
            } else {
                // 角度が180度を超える場合は反対方向に回転した方が近いので補正
                if (deltaY > std::numbers::pi_v<float>) {
                    deltaY -= tao;
                } else if (deltaY < -std::numbers::pi_v<float>) {
                    deltaY += tao;
                }
                bool signedB  = (deltaY > 0.f);
                float signedF = (deltaY > 0.f) ? 1.f : -1.f;
                destinationAngleXY[Y] -= signedF * (cameraController->getFixForForwardSpeed() * deltaTime);
                // 0を超えたら0にする
                if ((signedB && destinationAngleXY[Y] < forwardAngle) || (!signedB && destinationAngleXY[Y] > forwardAngle)) {
                    destinationAngleXY[Y] = forwardAngle;
                }
            }
        }

        // 角度制限を適用
        destinationAngleXY[X] = std::clamp(destinationAngleXY[X], cameraController->getMinRotateX(), cameraController->getMaxRotateX());
        // Y軸の角度制限を適用
        float forwardAngle    = std::atan2(cameraController->getForward()[X], cameraController->getForward()[Z]);
        destinationAngleXY[Y] = std::clamp(destinationAngleXY[Y], -cameraController->getAngleLimitY() + forwardAngle, cameraController->getAngleLimitY() + forwardAngle);

        cameraController->setDestinationAngleXY(destinationAngleXY);
    }
}
