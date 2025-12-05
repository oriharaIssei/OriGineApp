#include "TakeToGoalPosition.h"

/// ECS
// component
#include "component/transform/Transform.h"

TakeToGoalPosition::TakeToGoalPosition() : ISystem(SystemCategory::Initialize) {}
TakeToGoalPosition::~TakeToGoalPosition() {}

void TakeToGoalPosition::Initialize() {}
void TakeToGoalPosition::Finalize() {}

void TakeToGoalPosition::UpdateEntity(Entity* _entity) {
    Transform* transform = GetComponent<Transform>(_entity);

    Entity* goalPositionEntity = GetUniqueEntity("GoalPosition");
    if (goalPositionEntity) {
        Transform* goalPos = GetComponent<Transform>(goalPositionEntity);

        transform->translate = goalPos->GetWorldTranslate();
        transform->UpdateMatrix();
    }
}
