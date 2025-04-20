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
#include "component/Piller/FloatingFloorStatus.h"
#include "component/Piller/FloatingFloorSpawner.h"
// #include "component/Piller/PillerStates.h"
//  system
//  #include "system/FloorAndPillerColum/CreateFloorAndPillerSystem.h"
#include "system/FloatingFloor/CanageStateFallSystem.h"
#include "system/FloatingFloor/DeleteFloatingFloorSystem.h"
#include "system/FloatingFloor/FloatingFloorDamageSystem.h"
#include "system/FloatingFloor/FloatingFloorFallSystem.h"
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
        GameEntity* floatingFloor = CreateEntity<Transform, AABBCollider, Rigidbody, ModelMeshRenderer, FloatingFloorStatus>("Floor", Transform(), AABBCollider(), Rigidbody(), ModelMeshRenderer(), FloatingFloorStatus());
        // ================================= Componentを初期化 ================================= //

        Transform* transform = getComponent<Transform>(floatingFloor); // 柱
        Transform* baseTransform  = getComponent<Transform>(_entity); // 床
        if (!baseTransform) {
            return;
        }
        // ランダムでセーフゾーンがあるかを設定（セーフゾーンありがコスト1,セーフゾーンなしがコスト0）Maxコスト2

        //* Transform
        transform->translate = Vec3f(baseTransform->translate[X], floatFloorSpawner->GetPositionHeight(), baseTransform->translate[Z]);
        transform->scale     = baseTransform->scale;
       

        //* Collider

        //// AABB
        AABBCollider* aabbCollider             = getComponent<AABBCollider>(floatingFloor);
       /* aabbCollider->          = floatFloorSpawner->GetFallCollisionCenterPos();*/
        aabbCollider->getLocalShapePtr()->min_ = floatFloorSpawner->GetFallCollisionSizeMin();
        aabbCollider->getLocalShapePtr()->max_ = floatFloorSpawner->GetFallCollisionSizeMax();
        // aabbCollider->setActive(false);// 非アクティブにする

        // MeshRenderer
        ModelMeshRenderer* pillerRender = getComponent<ModelMeshRenderer>(floatingFloor);

        //// Model から MeshRenderer を作成
        // CreateModelMeshRenderer(pillerRender, piller, kApplicationResourceDirectory + "/Models/Piller", "Piller.obj");
        if (floatFloorSpawner->GetRowNumber() % 2 == 0) {
            CreateModelMeshRenderer(pillerRender, floatingFloor, kApplicationResourceDirectory + "/Models/whiteFloor", "whiteFloor.obj");
        } else {
            CreateModelMeshRenderer(pillerRender, floatingFloor, kApplicationResourceDirectory + "/Models/redFloor", "redFloor.obj");
        }
        // ↓ここのパラメータコピーがエディターで設定した値にならない(初期化設定した値は問題なし)

        // /// States

        // row,columNum
        FloatingFloorStatus* floatingFloorStatus = getComponent<FloatingFloorStatus>(floatingFloor);
        floatingFloorStatus->SetColumAndRow(0, floatFloorSpawner->GetRowNumber());

        // hp
        floatingFloorStatus->SetcurrentHP(floatFloorSpawner->GetHpMax());
        //prePos
        floatingFloorStatus->SetSavePos(floatFloorSpawner->GetPositionHeight());
        //fall
        floatingFloorStatus->SetFallSpeed(1.0f);
        floatingFloorStatus->SetFallPosY(-1.0f);
        floatingFloorStatus->SetIncrementFallEaseT(0.0f);

        // ================================= System ================================= //

        //------------------ Input
        // None

        //------------------ StateTransition
        ecs->getSystem<CanageStateFallSystem>()->addEntity(floatingFloor);
        ecs->getSystem<DeleteFloatingFloorSystem>()->addEntity(floatingFloor);
        //------------------ Movement
        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(floatingFloor);
        ecs->getSystem<FloatingFloorFallSystem>()->addEntity(floatingFloor);

       
        ecs->getSystem<UpdateMatrixSystem>()->addEntity(floatingFloor);

        //------------------ Collision
        ecs->getSystem<CollisionCheckSystem>()->addEntity(floatingFloor);
        ecs->getSystem<FloatingFloorDamageSystem>()->addEntity(floatingFloor);
        //------------------ Physics
        // None

        //------------------ Render
        ecs->getSystem<ColliderRenderingSystem>()->addEntity(floatingFloor);
        ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(floatingFloor);
     
    
}


void CreateFloorSystem::CostInit() {
    safeCost_   = 0;
    normalCost_ = 0;
}
