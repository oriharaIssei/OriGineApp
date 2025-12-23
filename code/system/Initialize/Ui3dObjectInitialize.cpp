#include "Ui3dObjectInitialize.h"

/// ECS
// component
#include "component/transform/Transform.h"

Ui3dObjectInitialize::Ui3dObjectInitialize() : ISystem(OriGine::SystemCategory::Initialize) {}

void Ui3dObjectInitialize::Initialize() {}
void Ui3dObjectInitialize::Finalize() {}

void Ui3dObjectInitialize::UpdateEntity(OriGine::Entity* _entity) {
    // カメラエンティティ取得
    OriGine::Entity* cameraEntity = GetUniqueEntity("GameCamera");
    if (!cameraEntity) {
        return;
    }

    auto* cameraTransform = GetComponent<OriGine::Transform>(cameraEntity);
    auto* transform       = GetComponent<OriGine::Transform>(_entity);
    if (!cameraTransform || !transform) {
        return;
    }

    // parentを結ぶ
    transform->parent = cameraTransform;
    transform->UpdateMatrix();
}
