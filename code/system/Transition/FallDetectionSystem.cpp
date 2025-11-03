#include "FallDetectionSystem.h"

#include "component/physics/Rigidbody.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"
#include "component/Stage/Stage.h"
#include "component/Stage/StageFloor.h"
#include "component/transform/Transform.h"

#include "logger/Logger.h"

FallDetectionSystem::FallDetectionSystem() : ISystem(SystemCategory::StateTransition) {}

FallDetectionSystem::~FallDetectionSystem() {
}

void FallDetectionSystem::Initialize() {
}

void FallDetectionSystem::Finalize() {
}

void FallDetectionSystem::UpdateEntity(Entity* _entity) {
    constexpr float kFallThresholdY = -26.0f;
    Transform* transform            = getComponent<Transform>(_entity);

    if (!transform) {
        return;
    }

    if (transform->worldMat[3][Y] < kFallThresholdY) {
        PlayerStatus* playerStatus = getComponent<PlayerStatus>(_entity);
        PlayerState* playerState   = getComponent<PlayerState>(_entity);

        StageFloor* stageFloor = getComponent<StageFloor>(getEntity(playerState->getLastFloorEntityIndex()));

        transform->translate = RetryPos(
            getComponent<Stage>(getUniqueEntity("Stage")),
            stageFloor,
            transform->translate);

        if (playerStatus) {
            playerStatus->Initialize(_entity);
        }
        if (playerState) {
            playerState->Initialize(_entity);
        }

        Rigidbody* rigidbody = getComponent<Rigidbody>(_entity);
        if (rigidbody) {
            rigidbody->setAcceleration(Vec3f(0.0f, 0.0f, 0.0f));
            rigidbody->setVelocity(Vec3f(0.0f, 0.0f, 0.0f));
            rigidbody->setUseGravity(true);
        }
    }
}

Vec3f FallDetectionSystem::RetryPos(Stage* _stage, StageFloor* _stageFloor, const Vec3f& _currentPos) {
    constexpr float kSpawnHeight   = 2.0f;
    constexpr float kSpawnEpsilonZ = 3.f;
    if (!_stage || !_stageFloor) {
        LOG_ERROR("Stage or StageFloor is nullptr. Returning default start position. \n Position: [ X: {}, Y: {}, Z: {}]", _currentPos[X], _currentPos[Y], _currentPos[Z]);

        Transform* startPos = getComponent<Transform>(getUniqueEntity("StartPosition"));
        if (startPos) {
            return startPos->translate;
        } else {
            LOG_ERROR("StartPosition entity not found. Returning (0,10,0) as fallback.");
            return Vec3f(0.0f, 10.0f, 0.0f);
        }
    }

    auto& controlPoints     = _stage->getControlPoints();
    const Stage::Link& link = _stage->getLinks()[_stageFloor->getLinkIndex()];
    Vec3f from              = controlPoints[_stageFloor->getFromPointIndex()].pos_ + Vec3f(0.f, link.height_ * 0.5f, 0.f);

    return from + Vec3f(0.0f, kSpawnHeight, kSpawnEpsilonZ);
}
