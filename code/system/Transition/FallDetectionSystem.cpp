#include "FallDetectionSystem.h"

#include "component/physics/Rigidbody.h"
#include "component/Player/PlayerStatus.h"
#include "component/transform/Transform.h"
#include "ECSManager.h"

FallDetectionSystem::FallDetectionSystem() : ISystem(SystemType::StateTransition) {}

FallDetectionSystem::~FallDetectionSystem() {
}

void FallDetectionSystem::Initialize() {
}

void FallDetectionSystem::Finalize() {
}

void FallDetectionSystem::UpdateEntity(GameEntity* _entity) {
    Transform* transform = getComponent<Transform>(_entity);

    if (!transform) {
        return;
    }

    // !TODO 汎用化
    if (transform->worldMat[3][Y] < -20.0f) {
        GameEntity* sartPositionEntity    = getUniqueEntity("StartPosition");
        Transform* startPositionTransform = getComponent<Transform>(sartPositionEntity);

        transform->translate       = startPositionTransform->translate;
        PlayerStatus* playerStatus = getComponent<PlayerStatus>(_entity);
        if (playerStatus) {
            playerStatus->Initialize(_entity);
        }
        Rigidbody* rigidbody = getComponent<Rigidbody>(_entity);
        if (rigidbody) {
            rigidbody->setAcceleration(Vec3f(0.0f, 0.0f, 0.0f));
            rigidbody->setVelocity(Vec3f(0.0f, 0.0f, 0.0f));
            rigidbody->setUseGravity(true);
        }
    }
}
