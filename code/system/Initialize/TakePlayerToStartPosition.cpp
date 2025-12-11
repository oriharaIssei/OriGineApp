#include "TakePlayerToStartPosition.h"

#include "component/transform/Transform.h"

TakePlayerToStartPosition::TakePlayerToStartPosition()
    : ISystem(OriGine::SystemCategory::Initialize) {}

TakePlayerToStartPosition::~TakePlayerToStartPosition() {}

void TakePlayerToStartPosition::Initialize() {}

void TakePlayerToStartPosition::Finalize() {}

void TakePlayerToStartPosition::UpdateEntity(OriGine::Entity* _entity) {
    OriGine::Transform* transform = GetComponent<OriGine::Transform>(_entity);
    if (!transform) {
        return;
    }
    OriGine::Entity* startPositionEntity = GetUniqueEntity("StartPosition");
    if (!startPositionEntity) {
        return;
    }
    OriGine::Transform* startPositionTransform = GetComponent<OriGine::Transform>(startPositionEntity);

    // 初期位置に移動
    transform->translate = startPositionTransform->translate;

    // 初期回転を設定（必要に応じて）
    transform->rotate = OriGine::Quaternion::Identity();

    // 初期スケールを設定（必要に応じて）
    transform->scale = {1.0f, 1.0f, 1.0f};
}
