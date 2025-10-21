#include "CreateStage.h"

/// engine
#define RESOURCE_DIRECTORY
#include "Engine.h"
#include "EngineInclude.h"
#include "scene/SceneManager.h"

/// ECS
// component
#include "component/collision/collider/AABBCollider.h"
#include "component/collision/collider/OBBCollider.h"
#include "component/collision/CollisionPushBackInfo.h"
#include "component/renderer/primitive/BoxRenderer.h"
#include "component/transform/Transform.h"
// system
#include "system/collision/CollisionCheckSystem.h"
#include "system/Initialize/TakePlayerToStartPosition.h"
#include "system/render/TexturedMeshRenderSystem.h"

#include "component/Stage/Stage.h"
#include "component/Stage/StageFloor.h"
#include "component/Stage/StageWall.h"

/// math
#include <DirectXMath.h>

void CreateStage::UpdateEntity(Entity* _entity) {

    auto stage = getComponent<Stage>(_entity);

    if (stage == nullptr) {
        return;
    }
    Transform* stageTransform = getComponent<Transform>(_entity);

    // Meshと当たり判定を作成
    auto& controlPoints = stage->getControlPoints();
    int32_t linkIndex   = 0;
    for (auto& link : stage->getLinks()) {
        Vec3f min, max, mid, diffToFrom, direction;

        auto& toPoint   = controlPoints[link.to_];
        auto& fromPoint = controlPoints[link.from_];

        Vec3f from          = fromPoint.pos_;
        Vec3f to            = toPoint.pos_;
        diffToFrom          = to - from;
        direction           = diffToFrom.normalize();
        mid                 = from + (diffToFrom * 0.5f);
        const Vec3f& normal = link.normal_.normalize();

        ///==========================================
        // Entity
        ///==========================================
        bool isWall = true;

        std::string entityName = "Wall";
        if (fabs(link.normal_[Y]) > 0.7f) {
            entityName = "Ground";
            isWall     = false;
        }
        int32_t index         = CreateEntity(entityName);
        Entity* createdEntity = getEntity(index);

        createdEntity->setShouldSave(false);

        ///==========================================
        // Tag
        ///==========================================
        if (isWall) {
            StageWall wallTag;
            wallTag.setLinkIndex(linkIndex);
            wallTag.setToPointIndex(link.to_);
            wallTag.setFromPointIndex(link.from_);
            wallTag.setWidth(link.width_);

            addComponent<StageWall>(createdEntity, wallTag);
        } else {
            StageFloor floorTag;
            floorTag.setLinkIndex(linkIndex);
            floorTag.setToPointIndex(link.to_);
            floorTag.setFromPointIndex(link.from_);
            floorTag.setWidth(link.width_);

            addComponent<StageFloor>(createdEntity, floorTag);
        }

        ///==========================================
        // Transform
        ///==========================================
        Transform entityTransformData;
        entityTransformData.translate = mid;
        entityTransformData.UpdateMatrix();
        addComponent<Transform>(createdEntity, entityTransformData);
        Transform* entityTransform = getComponent<Transform>(createdEntity);
        entityTransform->parent    = stageTransform;
        ///==========================================
        // Collider
        ///==========================================
        // aabbか OBBか

        bool isAABB          = std::fabs(direction[X]) > 0.999f || std::fabs(direction[Y]) > 0.999f || std::fabs(direction[Z]) > 0.999f;
        bool orthogonalAxisX = fabs(normal[X]) > 0.999f;
        bool orthogonalAxisY = fabs(normal[Y]) > 0.999f;
        isAABB               = isAABB && (orthogonalAxisX || orthogonalAxisY);

        if (isAABB) {
            if (orthogonalAxisX) {
                // X 軸に直交する壁 (YZ 平面に並行)
                // デフォルトが Y == 1 なので heightとwidthを入れ替える
                AABBCollider collider;
                collider.setActive(true);

                Vec3f halfSize = Vec3f(link.height_ * 0.5f, link.width_ * 0.5f, (diffToFrom[Z] * 0.5f) + Stage::kObjectMargin);
                min            = -halfSize;
                max            = halfSize;

                collider.setLocalMin(min);
                collider.setLocalMax(max);
                collider.setParent(entityTransform);

                addComponent<AABBCollider>(createdEntity, collider);
            } else {
                // Y 軸に直交する床/天井 (XZ 平面に並行)
                // これがデフォルト
                AABBCollider collider;
                collider.setActive(true);

                Vec3f halfSize = Vec3f(link.width_ * 0.5f, link.height_ * 0.5f, (diffToFrom[Z] * 0.5f) + Stage::kObjectMargin);
                min            = -halfSize;
                max            = halfSize;

                collider.setLocalMin(min);
                collider.setLocalMax(max);
                collider.setParent(entityTransform);

                addComponent<AABBCollider>(createdEntity, collider);
            }
        } else {
            // 傾いている壁は OBB で表現
            OBBCollider collider;
            collider.setActive(true);

            Vec3f halfSize = Vec3f(link.width_ * 0.5f, link.height_ * 0.5f, (diffToFrom.length() * 0.5f) + Stage::kObjectMargin);
            min            = -halfSize;
            max            = halfSize;

            collider.setLocalCenter(Vec3f(0.f, 0.f, 0.f));
            collider.setLocalHalfSize(halfSize);

            // forward, right, up から回転を作る
            Vec3f forward    = direction;
            Vec3f right      = forward.cross(normal).normalize();
            Vec3f up         = right.cross(forward).normalize();
            Matrix4x4 rotMat = {
                right[X], right[Y], right[Z], 0.f,
                up[X], up[Y], up[Z], 0.f,
                -forward[X], -forward[Y], -forward[Z], 0.f,
                0.f, 0.f, 0.f, 1.f};

            Quaternion rot          = rotMat.decomposeMatrixToQuaternion().normalize();
            entityTransform->rotate = rot;
            collider.setParent(entityTransform);

            addComponent<OBBCollider>(createdEntity, collider);
        }

        ///==========================================
        // BoxRenderer
        ///==========================================
        BoxRenderer renderer;
        Vec3f size = max - min;
        renderer.getPrimitive().setSize(size);
        renderer.setMaterialIndex(0);
        addComponent<BoxRenderer>(createdEntity, renderer);

        ///==========================================
        // Material
        ///==========================================
        Material material                = {};
        material.shininess_              = 50.f;
        material.enableLighting_         = true;
        material.environmentCoefficient_ = 0.05f;
        addComponent<Material>(createdEntity, material, false);

        ///==========================================
        // CollisionPushBackInfo
        ///==========================================
        CollisionPushBackInfo pushBackInfo;
        pushBackInfo.setPushBackType(CollisionPushBackType::PushBack);
        addComponent<CollisionPushBackInfo>(createdEntity, pushBackInfo);

        ///==========================================
        // 動作する システムに登録
        ///==========================================
        getScene()
            ->getSystem(nameof<CollisionCheckSystem>())
            ->addEntity(createdEntity);
        getScene()->getSystem(nameof<TexturedMeshRenderSystem>())->addEntity(createdEntity);

        ++linkIndex;
    }

    // goal
    int32_t goalIndex             = stage->getGoalPointIndex();
    goalIndex                     = std::clamp(goalIndex, 0, static_cast<int32_t>(controlPoints.size() - 1));
    Stage::ControlPoint goalPoint = controlPoints[goalIndex];

    SceneSerializer serializer(getScene());
    Entity* goalEntity = serializer.LoadEntity(kApplicationResourceDirectory + "/entities", "Goal");
    if (goalEntity == nullptr) {
        LOG_ERROR("Failed to load Goal entity.");
        return;
    }
    goalEntity->setShouldSave(false);
    Transform* goalTransform = getComponent<Transform>(goalEntity);
    goalTransform->translate = goalPoint.pos_;
    goalTransform->UpdateMatrix();

    // start
    int32_t startIndex                    = stage->getStartPointIndex();
    startIndex                            = std::clamp(startIndex, 0, static_cast<int32_t>(controlPoints.size() - 1));
    const Stage::ControlPoint& startPoint = controlPoints[startIndex];

    Transform startTransform;
    startTransform.translate = startPoint.pos_;
    startTransform.UpdateMatrix();
    int32_t startPosEntityId = CreateEntity("StartPosition", true);
    Entity* startPosEntity   = getEntity(startPosEntityId);
    startPosEntity->setShouldSave(false);
    addComponent<Transform>(startPosEntity, startTransform, false);
}
