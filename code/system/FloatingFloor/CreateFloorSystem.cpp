#include "CreateFloorSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
// lib
#include "input/Input.h"
// include
#include <Quaternion.h>
#include <Vector3.h>
// component
#include "component/Piller/FloatingFloorSpawner.h"
#include "component/Piller/FloatingFloorStatus.h"

//  system
#include "system/Block/BlockExBomCollision.h"
#include "system/Block/BreakBlockSystem.h"
#include "system/FloatingFloor/CanageStateFallSystem.h"
#include "system/FloatingFloor/DeleteFloatingFloorSystem.h"
#include "system/FloatingFloor/FloatingFloorDamageSystem.h"
#include "system/FloatingFloor/FloatingFloorFallSystem.h"
#include "system/FloatingFloor/FloatingFloorRevivalSystem.h"
#include "system/Floor/DeleteFloorSystem.h"
#include "system/Floor/FloorUpdateMatrixSystem.h"
#include "system/Matrix/UpdateMatrixSystem.h"

CreateFloorSystem::CreateFloorSystem() : ISystem(SystemType::Initialize) {}
CreateFloorSystem::~CreateFloorSystem() {}

void CreateFloorSystem::Initialize() {
    isInited_ = false;
}

void CreateFloorSystem::Finalize() {
}

void CreateFloorSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    floatFloorSpawner = getComponent<FloatingFloorSpawner>(_entity);

    if (!floatFloorSpawner) {
        return;
    }

    // 床生成
    /*CreateBottomFloor();*/
    CreateFloatingFloor(_entity);
    isInited_ = true;
}

void CreateFloorSystem::CreateFloatingFloor(GameEntity* _entity) {
    ECSManager* ecs = ECSManager::getInstance();

    // ================================= Bullet Entityを 生成 ================================= //
    GameEntity* floatingFloor = CreateEntity<Transform, AABBCollider, Rigidbody, ModelMeshRenderer, FloatingFloorStatus, Audio, Audio, Audio, Audio>("Floor", Transform(), AABBCollider(), Rigidbody(), ModelMeshRenderer(), FloatingFloorStatus(), Audio(), Audio(), Audio(), Audio());
    // ================================= Componentを初期化 ================================= //

    Transform* transform     = getComponent<Transform>(floatingFloor); // 柱
    Transform* baseTransform = getComponent<Transform>(_entity); // 床
    if (!baseTransform) {
        return;
    }
    // ランダムでセーフゾーンがあるかを設定（セーフゾーンありがコスト1,セーフゾーンなしがコスト0）Maxコスト2

    //* Transform
    transform->translate = Vec3f(baseTransform->translate[X], floatFloorSpawner->GetPositionHeight(), baseTransform->translate[Z]);
    transform->scale     = baseTransform->scale;

    //* Collider

    //// AABB
    AABBCollider* aabbCollider = getComponent<AABBCollider>(floatingFloor);
    /* aabbCollider->          = floatFloorSpawner->GetFallCollisionCenterPos();*/
    aabbCollider->getLocalShapePtr()->min_ = floatFloorSpawner->GetFallCollisionSizeMin();
    aabbCollider->getLocalShapePtr()->max_ = floatFloorSpawner->GetFallCollisionSizeMax();
    // aabbCollider->setActive(false);// 非アクティブにする

    // MeshRenderer
    ModelMeshRenderer* pillerRender = getComponent<ModelMeshRenderer>(floatingFloor);

    //// Model から MeshRenderer を作成
    // CreateModelMeshRenderer(pillerRender, piller, kApplicationResourceDirectory + "/Models/Piller", "Piller.obj");
    switch (floatFloorSpawner->GetRowNumber()) {
    case 0:
        CreateModelMeshRenderer(pillerRender, floatingFloor, kApplicationResourceDirectory + "/Models/Floor_Gold", "Floor_Gold.gltf");
        break;
    case 1:
        CreateModelMeshRenderer(pillerRender, floatingFloor, kApplicationResourceDirectory + "/Models/FloorSilver", "FloorSilver.gltf");
        break;
    case 2:
        CreateModelMeshRenderer(pillerRender, floatingFloor, kApplicationResourceDirectory + "/Models/Floor", "Floor.gltf");

        break;
    default:
        break;
    }
    // ↓ここのパラメータコピーがエディターで設定した値にならない(初期化設定した値は問題なし)

    // /// States

    // row,columNum
    FloatingFloorStatus* floatingFloorStatus = getComponent<FloatingFloorStatus>(floatingFloor);
    floatingFloorStatus->SetColumAndRow(0, floatFloorSpawner->GetRowNumber());
    floatingFloorStatus->SetRevivalTime(floatFloorSpawner->GetRevivalTime());

    ///*　パラメータべた書きゾーン

    // hp
    floatingFloorStatus->SetcurrentHP(floatFloorSpawner->GetHpMax());
    floatingFloorStatus->SetHPMax(floatFloorSpawner->GetHpMax());
    // pretrasfnorm
    floatingFloorStatus->SetStartPosY(floatFloorSpawner->GetPositionHeight());
    floatingFloorStatus->SetSaveScale(baseTransform->scale);
    // fall
    floatingFloorStatus->SetFallSpeed(1.0f);
    floatingFloorStatus->SetFallPosY(-1.0f);
    floatingFloorStatus->SetIncrementFallEaseT(0.0f);

    // ratio
    floatingFloorStatus->SetScoreUpRatio(floatFloorSpawner->GetRatio());

    for (int32_t i = 0; i < audios_.size(); ++i) {
        audios_[i]  = getComponent<Audio>(_entity, i); // audio
        faudios_[i] = getComponent<Audio>(floatingFloor, i); // audio

        faudios_[i] = audios_[i];
    }

    // ================================= System ================================= //

    //------------------ Input
    // None

    //------------------ StateTransition
    ecs->getSystem<CanageStateFallSystem>()->addEntity(floatingFloor);
    ecs->getSystem<DeleteFloatingFloorSystem>()->addEntity(floatingFloor);
    //------------------ Movement
    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(floatingFloor);
    ecs->getSystem<FloatingFloorFallSystem>()->addEntity(floatingFloor);
    ecs->getSystem<FloatingFloorRevivalSystem>()->addEntity(floatingFloor);
    ecs->getSystem<UpdateMatrixSystem>()->addEntity(floatingFloor);

    //------------------ Collision
    ecs->getSystem<CollisionCheckSystem>()->addEntity(floatingFloor);
    ecs->getSystem<FloatingFloorDamageSystem>()->addEntity(floatingFloor);
    //------------------ Physics
    // None

    //------------------ Render
    /*ecs->getSystem<ColliderRenderingSystem>()->addEntity(floatingFloor);*/
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(floatingFloor);
}

void CreateFloorSystem::CostInit() {
    safeCost_   = 0;
    normalCost_ = 0;
}
