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
    Transform* transform            = GetComponent<Transform>(_entity);

    if (!transform) {
        return;
    }

    if (transform->worldMat[3][Y] < kFallThresholdY) {
        PlayerStatus* playerStatus = GetComponent<PlayerStatus>(_entity);
        PlayerState* playerState   = GetComponent<PlayerState>(_entity);

        StageFloor* stageFloor = GetComponent<StageFloor>(GetEntity(playerState->GetLastFloorEntityIndex()));

        transform->translate = RetryPos(
            GetComponent<Stage>(GetUniqueEntity("Stage")),
            stageFloor,
            transform->translate);

        if (playerStatus) {
            playerStatus->Initialize(_entity);
        }
        if (playerState) {
            playerState->Initialize(_entity);
        }

        Rigidbody* rigidbody = GetComponent<Rigidbody>(_entity);
        if (rigidbody) {
            rigidbody->SetAcceleration(Vec3f(0.0f, 0.0f, 0.0f));
            rigidbody->SetVelocity(Vec3f(0.0f, 0.0f, 0.0f));
            rigidbody->SetUseGravity(true);
        }
    }
}

Vec3f FallDetectionSystem::RetryPos(Stage* _stage, StageFloor* _stageFloor, const Vec3f& _currentPos) {
    constexpr float kSpawnHeight   = 2.0f;
    constexpr float kSpawnEpsilonZ = 3.f;
    if (!_stage || !_stageFloor) {
        LOG_ERROR("Stage or StageFloor is nullptr. Returning default start position. \n Position: [ X: {}, Y: {}, Z: {}]", _currentPos[X], _currentPos[Y], _currentPos[Z]);

        Transform* startPos = GetComponent<Transform>(GetUniqueEntity("StartPosition"));
        if (startPos) {
            return startPos->translate;
        } else {
            LOG_ERROR("StartPosition entity not found. Returning (0,10,0) as fallback.");
            return Vec3f(0.0f, 10.0f, 0.0f);
        }
    }

    auto& controlPoints     = _stage->GetControlPoints();
    const Stage::Link& link = _stage->GetLinks()[_stageFloor->GetLinkIndex()];
    Vec3f from              = controlPoints[_stageFloor->GetFromPointIndex()].pos + Vec3f(0.f, link.height * 0.5f, 0.f);

    return from + Vec3f(0.0f, kSpawnHeight, kSpawnEpsilonZ);
}
