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
#include "system/Initialize/TakePlayerToStartPosition.h"
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

        Vec3f from = fromPoint.pos_ - Vec3f(link.width_ * 0.5f, link.height_ * 0.5f, 0.f);
        Vec3f to   = toPoint.pos_ + Vec3f(link.width_ * 0.5f, link.height_ * 0.5f, 0.f);

        min = MinElement(from, to);
        max = MaxElement(from, to);

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

        int32_t index          = CreateEntity("Wall");
        GameEntity* wallEntity = getEntity(index);
        wallEntity->setShouldSave(false);

        addComponent<BoxRenderer>(wallEntity, renderer);
        addComponent<AABBCollider>(wallEntity, collider);

        getScene()->getSystem(nameof<CollisionCheckSystem>())->addEntity(wallEntity);
        getScene()->getSystem(nameof<TexturedMeshRenderSystem>())->addEntity(wallEntity);
    }

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
    goalEntity->setShouldSave(false);
    Transform* goalTransform = getComponent<Transform>(goalEntity);
    goalTransform->translate = goalPoint.pos_;

    // start
    int32_t startIndex                    = stage->getStartPointIndex();
    startIndex                            = std::clamp(startIndex, 0, static_cast<int32_t>(controlPoints.size() - 1));
    const Stage::ControlPoint& startPoint = controlPoints[startIndex];

    int32_t startPosEntityID   = CreateEntity("StartPosition", true);
    GameEntity* startPosEntity = getEntity(startPosEntityID);
    Transform startPos;
    startPos.translate = startPoint.pos_;
    addComponent<Transform>(startPosEntity, startPos);
}
