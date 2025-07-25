#include "CreateStage.h"

/// engine
#include "Engine.h"
/// ECS
#include "ECSManager.h"
// component
#include "component/collider/Collider.h"
#include "component/renderer/primitive/Primitive.h"
#include "component/transform/Transform.h"
// system
#include "system/collision/CollisionCheckSystem.h"
#include "system/render/TexturedMeshRenderSystem.h"

#include "component/Stage.h"

void CreateStage::UpdateEntity(GameEntity* _entity) {
    auto stage = getComponent<Stage>(_entity);

    if (stage == nullptr) {
        return;
    }
    Transform* entityTransform = getComponent<Transform>(_entity);

    // Meshと当たり判定を作成
    auto& controlPoints = stage->getControlPoints();
    for (auto& link : stage->getLinks()) {
        Vec3f min, max;

        auto& toPoint   = controlPoints[link.to_];
        auto& fromPoint = controlPoints[link.from_];

        min = toPoint.pos_ - Vec3f(link.width_ * 0.5f, link.height_ * 0.5f, 0.f);
        max = fromPoint.pos_ + Vec3f(link.width_ * 0.5f, link.height_ * 0.5f, 0.f);

        // aabb
        AABBCollider collider;
        collider.setLocalMin(min);
        collider.setLocalMax(max);

        collider.setParent(entityTransform);

        // box
        BoxRenderer renderer;
        Vec3f size = max - min;
        renderer.getPrimitive().setSize(size);

        Vec3f mid = min + size * 0.5f;
        Transform transform;
        transform.parent    = entityTransform;
        transform.translate = mid;
        renderer.setTransform(transform);

        ECSManager::getInstance()->addComponent<BoxRenderer>(_entity->getID(), renderer);
        ECSManager::getInstance()->addComponent<AABBCollider>(_entity->getID(), collider);
    }

    ECSManager::getInstance()->getSystem<CollisionCheckSystem>(SystemType::Collision)->addEntity(_entity);
    ECSManager::getInstance()->getSystem<TexturedMeshRenderSystem>(SystemType::Render)->addEntity(_entity);
}
