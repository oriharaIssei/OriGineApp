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

void ShadowCastSystem::UpdateEntity(EntityHandle _entity) {
    auto* transform            = GetComponent<Transform>(_entity);
    auto* collsionPushBackInfo = GetComponent<CollisionPushBackInfo>(_entity);

    if (collsionPushBackInfo == nullptr) {
        LOG_ERROR("CollisionPushBackInfo component not found.");
        return;
    }

    float shadowHeight  = -FLT_MAX;
    float defaultHeight = transform->translate[Y];

    for (auto& [handle, info] : collsionPushBackInfo->GetCollisionInfoMap()) {
        Entity* otherEntity = GetEntity(handle);

        if (!otherEntity) {
            LOG_ERROR("Collided entity not found. \n Handle: {}", uuids::to_string(handle.uuid));
            continue;
        }
        auto* otherPushBackInfo = GetComponent<CollisionPushBackInfo>(handle);
        if (!otherPushBackInfo) {
            LOG_ERROR("Collided entity does not have CollisionPushBackInfo component. \n Handle: {}", uuids::to_string(handle.uuid));
            continue;
        }
        if (otherPushBackInfo->GetPushBackType() != CollisionPushBackType::PushBack) {
            continue;
        }

        AABBCollider* otherCollider = GetComponent<AABBCollider>(handle);
        if (!otherCollider) {
            LOG_ERROR("Collided entity does not have AABBCollider component. \n Handle: {}", uuids::to_string(handle.uuid));
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
