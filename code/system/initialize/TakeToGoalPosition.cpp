#include "TakeToGoalPosition.h"

/// ECS
// component
#include "component/transform/Transform.h"

TakeToGoalPosition::TakeToGoalPosition() : ISystem(OriGine::SystemCategory::Initialize) {}
TakeToGoalPosition::~TakeToGoalPosition() {}

void TakeToGoalPosition::Initialize() {}
void TakeToGoalPosition::Finalize() {}

void TakeToGoalPosition::UpdateEntity(OriGine::Entity* _entity) {
    OriGine::Transform* transform = GetComponent<OriGine::Transform>(_entity);

    OriGine::Entity* goalPositionEntity = GetUniqueEntity("GoalPosition");
    if (goalPositionEntity) {
        OriGine::Transform* goalPos = GetComponent<OriGine::Transform>(goalPositionEntity);

        transform->translate = goalPos->GetWorldTranslate();
        transform->UpdateMatrix();
    }
}
