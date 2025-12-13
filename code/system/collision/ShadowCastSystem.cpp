#include "ShadowCastSystem.h"

/// ECS
// component
#include "component/collision/collider/AABBCollider.h"
#include "component/collision/CollisionPushBackInfo.h"
#include "component/transform/Transform.h"

using namespace OriGine;

ShadowCastSystem::ShadowCastSystem() : ISystem(SystemCategory::Collision) {}

void ShadowCastSystem::Initialize() {}
void ShadowCastSystem::Finalize() {}

void ShadowCastSystem::UpdateEntity(OriGine::Entity* entity) {
    auto* transform = GetComponent<Transform>(entity);

    auto* collsionPushBackInfo = GetComponent<CollisionPushBackInfo>(entity);

    if (collsionPushBackInfo == nullptr) {
        LOG_ERROR("CollisionPushBackInfo component not found.");
        return;
    }

    float shadowHeight  = -FLT_MAX;
    float defaultHeight = transform->translate[Y];

    for (auto& [id, info] : collsionPushBackInfo->GetCollisionInfoMap()) {
        Entity* otherEntity = GetEntity(id);

        if (!otherEntity) {
            LOG_ERROR("Collided entity not found. ID: {}", id);
            continue;
        }
        AABBCollider* otherCollider = GetComponent<AABBCollider>(otherEntity);
        if (!otherCollider) {
            LOG_ERROR("Collided entity does not have AABBCollider component. ID: {}", id);
            continue;
        }

        float height = otherCollider->GetWorldShape().Max()[Y];

        if (height > shadowHeight && height < defaultHeight) {
            shadowHeight = height;
        }
    }
    transform->translate[Y] = shadowHeight;
    transform->UpdateMatrix();
}
