#include "TakePlayerToStartPosition.h"

#include "component/transform/Transform.h"

using namespace OriGine;

TakePlayerToStartPosition::TakePlayerToStartPosition()
    : ISystem(OriGine::SystemCategory::Initialize) {}

TakePlayerToStartPosition::~TakePlayerToStartPosition() {}

void TakePlayerToStartPosition::Initialize() {}

void TakePlayerToStartPosition::Finalize() {}

void TakePlayerToStartPosition::UpdateEntity(EntityHandle _handle) {
    Transform* transform = GetComponent<Transform>(_handle);
    if (!transform) {
        return;
    }
    EntityHandle startPositionEntity  = GetUniqueEntity("StartPosition");
    Transform* startPositionTransform = GetComponent<Transform>(startPositionEntity);
    if (!startPositionTransform) {
        return;
    }

    // 初期位置に移動
    transform->translate = startPositionTransform->translate;

    // 初期回転を設定（必要に応じて）
    transform->rotate = OriGine::Quaternion::Identity();

    // 初期スケールを設定（必要に応じて）
    transform->scale = {1.0f, 1.0f, 1.0f};
}
