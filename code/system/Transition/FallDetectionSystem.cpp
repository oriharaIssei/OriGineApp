#include "FallDetectionSystem.h"

#include "component/physics/Rigidbody.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"
#include "component/transform/Transform.h"

#include "logger/Logger.h"

FallDetectionSystem::FallDetectionSystem() : ISystem(SystemCategory::StateTransition) {}

FallDetectionSystem::~FallDetectionSystem() {
}

void FallDetectionSystem::Initialize() {
}

void FallDetectionSystem::Finalize() {
}

void FallDetectionSystem::UpdateEntity(GameEntity* _entity) {
    constexpr float kFallThresholdY = -20.0f;
    Transform* transform            = getComponent<Transform>(_entity);

    if (!transform) {
        return;
    }

    if (transform->worldMat[3][Y] < kFallThresholdY) {
        PlayerStatus* playerStatus = getComponent<PlayerStatus>(_entity);
        PlayerState* playerState   = getComponent<PlayerState>(_entity);

        if (playerStatus) {
            playerStatus->Initialize(_entity);
        }
        if (playerState) {
            playerState->Initialize(_entity);
        }

        GameEntity* startPositionEntity = getUniqueEntity("StartPosition");
        if (!startPositionEntity) {
            return;
        }
        Transform* startPositionTransform = getComponent<Transform>(startPositionEntity);

        // 初期位置に移動
        transform->translate = startPositionTransform->translate;
        transform->UpdateMatrix();

        Rigidbody* rigidbody = getComponent<Rigidbody>(_entity);
        if (rigidbody) {
            rigidbody->setAcceleration(Vec3f(0.0f, 0.0f, 0.0f));
            rigidbody->setVelocity(Vec3f(0.0f, 0.0f, 0.0f));
            rigidbody->setUseGravity(true);
        }
    }
}
