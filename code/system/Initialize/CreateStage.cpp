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

    auto stage = GetComponent<Stage>(_entity);

    if (stage == nullptr) {
        return;
    }
    Transform* stageTransform = GetComponent<Transform>(_entity);

    // Meshと当たり判定を作成
    auto& controlPoints = stage->GetControlPoints();

    if (controlPoints.empty()) {
        LOG_WARN("Stage has no control points.");
        return;
    }

    int32_t linkIndex = 0;
    for (auto& link : stage->GetLinks()) {
        Vec3f min, max, mid, diffToFrom, direction;

        auto& toPoint   = controlPoints[link.to];
        auto& fromPoint = controlPoints[link.from];

        Vec3f from          = fromPoint.pos;
        Vec3f to            = toPoint.pos;
        diffToFrom          = to - from;
        direction           = diffToFrom.normalize();
        mid                 = from + (diffToFrom * 0.5f);
        const Vec3f& normal = link.normal.normalize();

        ///==========================================
        // Entity
        ///==========================================
        bool isWall = true;

        std::string entityName = "Wall";
        if (fabs(link.normal[Y]) > 0.7f) {
            entityName = "Ground";
            isWall     = false;
        }
        int32_t index         = CreateEntity(entityName);
        Entity* createdEntity = GetEntity(index);

        createdEntity->SetShouldSave(false);

        ///==========================================
        // Tag
        ///==========================================
        if (isWall) {
            StageWall wallTag;
            wallTag.SetLinkIndex(linkIndex);
            wallTag.SetToPointIndex(link.to);
            wallTag.SetFromPointIndex(link.from);
            wallTag.SetWidth(link.width);

            AddComponent<StageWall>(createdEntity, wallTag);
        } else {
            StageFloor floorTag;
            floorTag.SetLinkIndex(linkIndex);
            floorTag.SetToPointIndex(link.to);
            floorTag.SetFromPointIndex(link.from);
            floorTag.SetWidth(link.width);

            AddComponent<StageFloor>(createdEntity, floorTag);
        }

        ///==========================================
        // Transform
        ///==========================================
        Transform entityTransformData;
        entityTransformData.translate = mid;
        entityTransformData.UpdateMatrix();
        AddComponent<Transform>(createdEntity, entityTransformData);
        Transform* entityTransform = GetComponent<Transform>(createdEntity);
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
                collider.SetActive(true);

                Vec3f halfSize = Vec3f(link.height * 0.5f, link.width * 0.5f, (diffToFrom[Z] * 0.5f) + Stage::kObjectMargin);
                min            = -halfSize;
                max            = halfSize;

                collider.SetLocalMin(min);
                collider.SetLocalMax(max);
                collider.SetParent(entityTransform);

                AddComponent<AABBCollider>(createdEntity, collider);
            } else {
                // Y 軸に直交する床/天井 (XZ 平面に並行)
                // これがデフォルト
                AABBCollider collider;
                collider.SetActive(true);

                Vec3f halfSize = Vec3f(link.width * 0.5f, link.height * 0.5f, (diffToFrom[Z] * 0.5f) + Stage::kObjectMargin);
                min            = -halfSize;
                max            = halfSize;

                collider.SetLocalMin(min);
                collider.SetLocalMax(max);
                collider.SetParent(entityTransform);

                AddComponent<AABBCollider>(createdEntity, collider);
            }
        } else {
            // 傾いている壁は OBB で表現
            OBBCollider collider;
            collider.SetActive(true);

            Vec3f halfSize = Vec3f(link.width * 0.5f, link.height * 0.5f, (diffToFrom.length() * 0.5f) + Stage::kObjectMargin);
            min            = -halfSize;
            max            = halfSize;

            collider.SetLocalCenter(Vec3f(0.f, 0.f, 0.f));
            collider.SetLocalHalfSize(halfSize);

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
            collider.SetParent(entityTransform);

            AddComponent<OBBCollider>(createdEntity, collider);
        }

        ///==========================================
        // BoxRenderer
        ///==========================================
        BoxRenderer renderer;
        Vec3f size                        = max - min;
        renderer.GetPrimitive().halfSize_ = size * 0.5f;
        renderer.SetMaterialIndex(0);
        AddComponent<BoxRenderer>(createdEntity, renderer);

        ///==========================================
        // Material
        ///==========================================
        Material material                = {};
        material.shininess_              = 50.f;
        material.enableLighting_         = true;
        material.environmentCoefficient_ = 0.05f;
        AddComponent<Material>(createdEntity, material, false);

        ///==========================================
        // CollisionPushBackInfo
        ///==========================================
        CollisionPushBackInfo pushBackInfo;
        pushBackInfo.SetPushBackType(CollisionPushBackType::PushBack);
        AddComponent<CollisionPushBackInfo>(createdEntity, pushBackInfo);

        ///==========================================
        // 動作する システムに登録
        ///==========================================
        ISystem* collisionSystem = GetScene()->GetSystem(nameof<CollisionCheckSystem>());
        if (collisionSystem) {
            collisionSystem->AddEntity(createdEntity);
        } else {
            LOG_INFO("This scene don't have CollisionCheckSystem");
        }
        ISystem* renderSystem = GetScene()->GetSystem(nameof<TexturedMeshRenderSystem>());
        if (renderSystem) {
            renderSystem->AddEntity(createdEntity);
        } else {
            LOG_INFO("This scene don't have TexturedMeshRenderSystem");
        }

        entityTransform->UpdateMatrix();

        ++linkIndex;
    }

    // goal
    int32_t goalIndex             = stage->GetGoalPointIndex();
    goalIndex                     = std::clamp(goalIndex, 0, static_cast<int32_t>(controlPoints.size() - 1));
    Stage::ControlPoint goalPoint = controlPoints[goalIndex];

    SceneSerializer serializer(GetScene());
    Entity* goalEntity = serializer.LoadEntity(kApplicationResourceDirectory + "/entities", "Goal");
    if (goalEntity == nullptr) {
        LOG_ERROR("Failed to load Goal entity.");
        return;
    }
    goalEntity->SetShouldSave(false);
    Transform* goalTransform = GetComponent<Transform>(goalEntity);
    goalTransform->translate = goalPoint.pos;
    goalTransform->UpdateMatrix();

    // start
    int32_t startIndex                    = stage->GetStartPointIndex();
    startIndex                            = std::clamp(startIndex, 0, static_cast<int32_t>(controlPoints.size() - 1));
    const Stage::ControlPoint& startPoint = controlPoints[startIndex];

    Transform startTransform;
    startTransform.translate = startPoint.pos;
    startTransform.UpdateMatrix();
    int32_t startPosEntityId = CreateEntity("StartPosition", true);
    Entity* startPosEntity   = GetEntity(startPosEntityId);
    startPosEntity->SetShouldSave(false);
    AddComponent<Transform>(startPosEntity, startTransform, false);
}
