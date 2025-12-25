#include "TakeToGoalPosition.h"

/// ECS
// component
#include "component/transform/Transform.h"

using namespace OriGine;

TakeToGoalPosition::TakeToGoalPosition() : ISystem(SystemCategory::Initialize) {}
TakeToGoalPosition::~TakeToGoalPosition() {}

void TakeToGoalPosition::Initialize() {}
void TakeToGoalPosition::Finalize() {}

void TakeToGoalPosition::UpdateEntity(EntityHandle _handle) {
    Transform* transform = GetComponent<Transform>(_handle);

    EntityHandle goalPositionEntityHandle = GetUniqueEntity("GoalPosition");
    Transform* goalPos                    = GetComponent<OriGine::Transform>(goalPositionEntityHandle);

    if (goalPos) {
        transform->translate = goalPos->GetWorldTranslate();
        transform->UpdateMatrix();
    }
}
