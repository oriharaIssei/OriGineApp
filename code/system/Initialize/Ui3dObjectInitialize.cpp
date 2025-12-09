#include "Ui3dObjectInitialize.h"

/// ECS
// component
#include "component/transform/Transform.h"

Ui3dObjectInitialize::Ui3dObjectInitialize() : ISystem(SystemCategory::Initialize) {}

void Ui3dObjectInitialize::Initialize() {}
void Ui3dObjectInitialize::Finalize() {}

void Ui3dObjectInitialize::UpdateEntity(Entity* _entity) {
    // カメラエンティティ取得
    Entity* cameraEntity = GetUniqueEntity("GameCamera");
    if (!cameraEntity) {
        return;
    }

    auto* cameraTransform = GetComponent<Transform>(cameraEntity);
    auto* transform       = GetComponent<Transform>(_entity);
    if (!cameraTransform || !transform) {
        return;
    }

    // parentを結ぶ
    transform->parent = cameraTransform;
}
