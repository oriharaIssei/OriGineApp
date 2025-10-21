#include "TakePlayerToStartPosition.h"

#include "component/transform/Transform.h"


TakePlayerToStartPosition::TakePlayerToStartPosition()
    : ISystem(SystemCategory::Initialize) {}

TakePlayerToStartPosition::~TakePlayerToStartPosition() {}

void TakePlayerToStartPosition::Initialize() {}

void TakePlayerToStartPosition::Finalize() {}

void TakePlayerToStartPosition::UpdateEntity(Entity* _entity) {
    Transform* transform = getComponent<Transform>(_entity);
    if (!transform) {
        return;
    }
    Entity* startPositionEntity    = getUniqueEntity("StartPosition");
    if (!startPositionEntity) {
        return;
    }
    Transform* startPositionTransform = getComponent<Transform>(startPositionEntity);

    // 初期位置に移動
    transform->translate = startPositionTransform->translate;

    // 初期回転を設定（必要に応じて）
    transform->rotate = Quaternion::Identity();

    // 初期スケールを設定（必要に応じて）
    transform->scale = {1.0f, 1.0f, 1.0f};
}
