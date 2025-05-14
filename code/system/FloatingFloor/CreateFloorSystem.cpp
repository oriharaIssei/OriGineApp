#include "CreateFloorSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
// lib
// include
#include <Vector3.h>
// component
#include "component/FloorUI/FloorUIController.h"
#include "component/FloorUI/FloorUIStatus.h"
#include "component/Piller/FloatingFloorSpawner.h"
#include "component/Piller/FloatingFloorStatus.h"
//  system
#include "system/FloatingFloor/CanageStateFallSystem.h"
#include "system/FloatingFloor/DeleteFloatingFloorSystem.h"
#include "system/FloatingFloor/FloatingFloorDamageSystem.h"
#include "system/FloatingFloor/FloatingFloorFallSystem.h"
#include "system/FloatingFloor/FloatingFloorRevivalSystem.h"
#include "system/Matrix/UpdateMatrixSystem.h"
#include"system/FloorUI/FloorUISystem.h"

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
    CreateFloatingFloor(_entity);
    isInited_ = true;
}

void CreateFloorSystem::CreateFloatingFloor(GameEntity* _entity) {

    // ポーズ中は通さない
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* floorUIControllerEntity            = ecsManager->getUniqueEntity("FloorUIController");

    if (!floorUIControllerEntity) {
        return;
    }

    FloorUIController* floorUIController = getComponent<FloorUIController>(floorUIControllerEntity);

    ECSManager* ecs = ECSManager::getInstance();

    // ================================= Bullet Entityを 生成 ================================= //
    GameEntity* floatingFloor = CreateEntity<Transform, AABBCollider, Rigidbody, ModelMeshRenderer, FloatingFloorStatus, Audio, Audio, Audio, Audio>("Floor", Transform(), AABBCollider(), Rigidbody(), ModelMeshRenderer(), FloatingFloorStatus(), Audio(), Audio(), Audio(), Audio());
    GameEntity* floorUIEntity = CreateEntity<Transform, Rigidbody, ModelMeshRenderer, FloorUIStatus>("FloorUI", Transform(), Rigidbody(), ModelMeshRenderer(), FloorUIStatus());
    // ================================= Componentを初期化 ================================= //

    Transform* transform        = getComponent<Transform>(floatingFloor); // 柱
    Transform* baseTransform    = getComponent<Transform>(_entity); // 床
    Transform* floorUITransform = getComponent<Transform>(floorUIEntity); // 床

    if (!baseTransform) {
        return;
    }

    //* Transform
    transform->translate = Vec3f(baseTransform->translate[X], floatFloorSpawner->GetPositionHeight(), baseTransform->translate[Z]);
    transform->scale     = baseTransform->scale;

    // setparent
    floorUITransform->parent = transform;

    //* Collider

    //// AABB
    AABBCollider* aabbCollider = getComponent<AABBCollider>(floatingFloor);
    /* aabbCollider->          = floatFloorSpawner->GetFallCollisionCenterPos();*/
    aabbCollider->getLocalShapePtr()->min_ = floatFloorSpawner->GetFallCollisionSizeMin();
    aabbCollider->getLocalShapePtr()->max_ = floatFloorSpawner->GetFallCollisionSizeMax();
    // aabbCollider->setActive(false);// 非アクティブにする

    // MeshRenderer
    ModelMeshRenderer* pillerRender  = getComponent<ModelMeshRenderer>(floatingFloor);
    ModelMeshRenderer* floorUIRender = getComponent<ModelMeshRenderer>(floorUIEntity);

    switch (floatFloorSpawner->GetRowNumber()) {
    case 0:
        CreateModelMeshRenderer(pillerRender, floatingFloor, kApplicationResourceDirectory + "/Models/Floor_Gold", "Floor_Gold.obj");
        break;
    case 1:
        CreateModelMeshRenderer(pillerRender, floatingFloor, kApplicationResourceDirectory + "/Models/FloorSilver", "FloorSilver.obj");
        break;
    case 2:
        CreateModelMeshRenderer(pillerRender, floatingFloor, kApplicationResourceDirectory + "/Models/Floor", "Floor.obj");

        break;
    default:
        break;
    }

    CreateModelMeshRenderer(floorUIRender, floorUIEntity, kApplicationResourceDirectory + "/Models/plane", "plane.obj");
    floorUIRender->setTexture(0, kApplicationResourceDirectory + "/Texture/UI/FloorUIArrow.png");

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
    floatingFloorStatus->SetFallPosY(-5.0f);
    floatingFloorStatus->SetIncrementFallEaseT(0.0f);

    // ratio
    floatingFloorStatus->SetScoreUpRatio(floatFloorSpawner->GetScoreUPRate());
    floatingFloorStatus->SetStartScoreUPRate(floatFloorSpawner->GetScoreUPRate());

    //collision
    floatingFloorStatus->SetFallCollisionCenterPos(floatFloorSpawner->GetFallCollisionCenterPos());
    floatingFloorStatus->SetFallCollisionSizeMax(floatFloorSpawner->GetFallCollisionSizeMax());
    floatingFloorStatus->SetFallCollisionSizeMin(floatFloorSpawner->GetFallCollisionSizeMin());


    for (int32_t i = 0; i < audios_.size(); ++i) {
        audios_[i]  = getComponent<Audio>(_entity, i); // audio
        faudios_[i] = getComponent<Audio>(floatingFloor, i); // audio

        faudios_[i] = audios_[i];
    }

    FloorUIStatus* floorUIStatus = getComponent<FloorUIStatus>(floorUIEntity);
    floorUIStatus->SetMoveEasing(floorUIController->GetMoveEasing());
    floorUIStatus->SetOpenEasing(floorUIController->GetOpenEasing());
    floorUIStatus->SetCloseEasing(floorUIController->GetCloseEasing());

    floorUIStatus->SetIsAnimation(&floatingFloorStatus->GetIsPlayerUnderTheFloor());


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

    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(floorUIEntity);
    ecs->getSystem<FloorUISystem>()->addEntity(floorUIEntity);
    //------------------ Collision
    ecs->getSystem<CollisionCheckSystem>()->addEntity(floatingFloor);
    ecs->getSystem<FloatingFloorDamageSystem>()->addEntity(floatingFloor);
    //------------------ Physics
    // None

    //------------------ Render
    /*ecs->getSystem<ColliderRenderingSystem>()->addEntity(floatingFloor);*/
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(floatingFloor);
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(floorUIEntity);
}

void CreateFloorSystem::CostInit() {
    safeCost_   = 0;
    normalCost_ = 0;
}
