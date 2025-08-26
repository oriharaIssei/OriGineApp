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
        ///==========================================
        // Entity
        ///==========================================

        std::string entityName = "Wall";
        if (link.normal_[Y] < 7.f) {
            entityName = "Ground";
        }
        int32_t index             = CreateEntity(entityName);
        GameEntity* createdEntity = getEntity(index);

        createdEntity->setShouldSave(false);

        ///==========================================
        // Collider
        ///==========================================

        Vec3f min, max;

        auto& toPoint   = controlPoints[link.to_];
        auto& fromPoint = controlPoints[link.from_];

        Vec3f from = fromPoint.pos_;
        Vec3f to   = toPoint.pos_;

        min = fromPoint.pos_ - Vec3f(link.width_ * 0.5f, link.height_ * 0.5f, 0.f);
        max = toPoint.pos_ + Vec3f(link.width_ * 0.5f, link.height_ * 0.5f, 0.f);

        const Vec3f& normal = link.normal_.normalize();
       
        // aabbか OBBか
        if (fabs(normal[X]) > 0.999f) {
            // X 軸に直交する壁 (YZ 平面に並行)
            AABBCollider collider;

            min = Vec3f(from[X] - link.width_ * 0.5f,
                from[Y] - link.height_ * 0.5f,
                from[Z]);
            max = Vec3f(to[X] + link.width_ * 0.5f,
                to[Y] + link.height_ * 0.5f,
                to[Z]);

            collider.setLocalMin(min);
            collider.setLocalMax(max);
            collider.setParent(entityTransform);

            addComponent<AABBCollider>(createdEntity, collider);

        } else if (fabs(normal[Y]) > 0.999f) {
            // Y 軸に直交する床/天井 (XZ 平面に並行)
            AABBCollider collider;

            min = Vec3f(from[X] - link.width_ * 0.5f,
                from[Y],
                from[Z] - link.height_ * 0.5f);
            max = Vec3f(to[X] + link.width_ * 0.5f,
                to[Y],
                to[Z] + link.height_ * 0.5f);

            collider.setLocalMin(min);
            collider.setLocalMax(max);
            collider.setParent(entityTransform);

            addComponent<AABBCollider>(createdEntity, collider);

        } else if (fabs(normal[Z]) > 0.999f) {
            // Z 軸に直交する壁 (XY 平面に並行)
            AABBCollider collider;

            min = Vec3f(from[X] - link.width_ * 0.5f,
                from[Y] - link.height_ * 0.5f,
                from[Z]);
            max = Vec3f(to[X] + link.width_ * 0.5f,
                to[Y] + link.height_ * 0.5f,
                to[Z]);

            collider.setLocalMin(min);
            collider.setLocalMax(max);
            collider.setParent(entityTransform);

            addComponent<AABBCollider>(createdEntity, collider);

        } else {
            // 傾いている壁は OBB で表現
            OBBCollider collider;
            Vec3f center   = (from + to) * 0.5f;
            Vec3f halfSize = Vec3f(link.width_ * 0.5f, link.height_ * 0.5f, Vec3f(to - from).length() * 0.5f);

            collider.setLocalCenter(center);
            collider.setLocalHalfSize(halfSize);

            // forward, right, up から回転を作る
            Vec3f forward = Vec3f(to - from).normalize();
            Vec3f right   = forward.cross(normal).normalize();
            Vec3f up      = right.cross(forward).normalize();

            Matrix4x4 rotMat = {
                right[X], right[Y], right[Z], 0.f,
                up[X],    up[Y],    up[Z],    0.f,
                -forward[X], -forward[Y], -forward[Z], 0.f,
                0.f,      0.f,      0.f,      1.f};
            Quaternion rot = Quaternion::FromMatrix(rotMat);

            collider.setRotate(rot);
            collider.setParent(entityTransform);

            addComponent<OBBCollider>(createdEntity, collider);
        }

        ///==========================================
        // BoxRenderer
        ///==========================================
        BoxRenderer renderer;
        Vec3f size = max - min;
        renderer.getPrimitive().setSize(size);

        Vec3f mid = min + size * 0.5f;
        Transform transform;
        transform.parent    = entityTransform;
        transform.translate = mid;
        renderer.setTransform(transform);

        addComponent<BoxRenderer>(createdEntity, renderer);

        getScene()->getSystem(nameof<CollisionCheckSystem>())->addEntity(createdEntity);
        getScene()->getSystem(nameof<TexturedMeshRenderSystem>())->addEntity(createdEntity);
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

    GameEntity* startPosEntity = getUniqueEntity("StartPosition");
    Transform startPos;
    startPos.translate = startPoint.pos_;
    addComponent<Transform>(startPosEntity, startPos);
}
