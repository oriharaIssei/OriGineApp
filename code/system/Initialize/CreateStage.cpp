#include "CreateStage.h"

/// engine
#define RESOURCE_DIRECTORY
#include "Engine.h"
#include "EngineInclude.h"
#include "scene/SceneManager.h"

/// ECS
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

        addComponent<BoxRenderer>(_entity, renderer);
        addComponent<AABBCollider>(_entity, collider);
    }

    getScene()->getSystem(nameof<CollisionCheckSystem>())->addEntity(_entity);
    getScene()->getSystem(nameof<TexturedMeshRenderSystem>())->addEntity(_entity);

    // goal
    int32_t goalIndex             = stage->getGoalPointIndex();
    goalIndex                     = std::clamp(goalIndex, 0, static_cast<int32_t>(controlPoints.size() - 1));
    Stage::ControlPoint goalPoint = controlPoints[goalIndex];

    SceneSerializer serializer(getScene());
    GameEntity* goalEntity = serializer.LoadEntity(kApplicationResourceDirectory + "/entities", "Goal");
    if (goalEntity == nullptr) {
        LOG_ERROR("Failed to load Goal entity.");
        return;
    }
    Transform* goalTransform = getComponent<Transform>(goalEntity);
    goalTransform->translate = goalPoint.pos_;
}
