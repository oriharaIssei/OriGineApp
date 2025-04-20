#include "TowerPositionSet.h"

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
#include "component/Floor/BottomFloorStates.h"
#include "component/Floor/FloorStates.h"
#include "component/Piller/PillerSpawner.h"
#include "component/Piller/FloatingFloorStatus.h"
// #include "component/Piller/PillerStates.h"
//  system
//  #include "system/FloorAndPillerColum/CreateFloorAndPillerSystem.h"
#include "system/Floor/DeleteFloorSystem.h"
#include "system/Floor/FloorUpdateMatrixSystem.h"
#include "system/FloorAndPiller/FloorAndPillerFallSystem.h"
#include "system/Piller/CanageStateFallSystem.h"
#include "system/Piller/DeletePillerEntitySystem.h"
#include "system/Piller/PillerDamageSystem.h"
#include "system/Piller/pillerUpdateMatrixSystem.h"

TowerPositionSet::TowerPositionSet() : ISystem(SystemType::Initialize) {}
TowerPositionSet::~TowerPositionSet() {}

void TowerPositionSet::Initialize() {
    isInited_ = false;
}

void TowerPositionSet::Finalize() {
}

void TowerPositionSet::UpdateEntity(GameEntity* _entity) {
    if (!_entity || isInited_) {
        return;
    }

    bottomFloorStates_ = getComponent<BottomFloorStates>(_entity);

    floorStates_ = getComponent<FloorStates>(_entity);
    /* pillerStates_          = getComponent<PillerStates>(_entity);*/
    pillerSpawner = getComponent<FloorAndPillerSpawner>(_entity);

    if (!bottomFloorStates_ || !floorStates_ || !pillerSpawner) {
        return;
    }

    /*floorModeCreater_.resize(bottomFloorStates_->GetFloorNum());

    for (int32_t i = 0; i < floorModeCreater_.size(); ++i) {
        floorModeCreater_[i] = getComponent<FloorModeCreater>(_entity);
        if (!floorModeCreater_[i]) {
            return;
        }
    }*/

   /* float moveRadius = bottomFloorStates_->GetFieldRadius();*/
    // 床生成
    CreateBottomFloor();
    CreateFloatingFloor();
    isInited_ = true;
}

void TowerPositionSet::CreateFloatingFloor() {
    ECSManager* ecs = ECSManager::getInstance();

    for (int32_t i = 0; i < pillerSpawner->GetColumNumMax(); ++i) {
        CostInit();
        for (int32_t j = 0; j < bottomFloorStates_->GetFloorNum(); ++j) {

            // ================================= Bullet Entityを 生成 ================================= //
            GameEntity* floor = CreateEntity<Transform, Transform, AABBCollider, Rigidbody, ModelMeshRenderer, FloatingFloorStatus>("Floor", Transform(), Transform(), AABBCollider(), Rigidbody(), ModelMeshRenderer(), FloatingFloorStatus());
            // ================================= Componentを初期化 ================================= //

            Transform* pillerTransform     = getComponent<Transform>(floor); // 柱
            Transform* pillerBaseTransform = getComponent<Transform>(floor, 1);
            //Transform* floorTransform      = getComponent<Transform>(floor); // 床

            // ランダムでセーフゾーンがあるかを設定（セーフゾーンありがコスト1,セーフゾーンなしがコスト0）Maxコスト2

            // 　親のY位置を設定
            if (i == 0) {
                pillerBaseTransform->translate = Vec3f(0.0f, 0.0f, 0.0f);

            } else {
                pillerBaseTransform->translate = Vec3f(0.0f, 0.0f + (pillerSpawner->GetPillerSpace() * float(i)), 0.0f);
            }

            SetXPosition(pillerBaseTransform, pillerTransform, Vec3f(0.0f, 0.0f, 0.0f), j);
         
            //* Collider
          
            //// AABB
            AABBCollider* aabbCollider             = getComponent<AABBCollider>(floor);
            aabbCollider->getLocalShapePtr()->min_ = pillerSpawner->GetFallCollisionSizeMin();
            aabbCollider->getLocalShapePtr()->max_ = pillerSpawner->GetFallCollisionSizeMax();
            //aabbCollider->setActive(false);// 非アクティブにする

            // MeshRenderer
            ModelMeshRenderer* pillerRender = getComponent<ModelMeshRenderer>(floor);
          

            //// Model から MeshRenderer を作成
            //CreateModelMeshRenderer(pillerRender, piller, kApplicationResourceDirectory + "/Models/Piller", "Piller.obj");
            if (j % 2 == 0) {
                CreateModelMeshRenderer(pillerRender, floor, kApplicationResourceDirectory + "/Models/whiteFloor", "whiteFloor.obj");
            } else {
                CreateModelMeshRenderer(pillerRender, floor, kApplicationResourceDirectory + "/Models/redFloor", "redFloor.obj");
            }
            // ↓ここのパラメータコピーがエディターで設定した値にならない(初期化設定した値は問題なし)

           // /// States

            // row,columNum
            FloatingFloorStatus* statusFandP = getComponent<FloatingFloorStatus>(floor);
            statusFandP->SetColumAndRow(i, j);

            //// 親ステータスをもらう
            //statusFloor->SetParentStatus(statusFandP);
          
            // savePosを設定
            statusFandP->SetSavePos(pillerBaseTransform->translate[Y]);
            // 落ちるオフセットを決める
            statusFandP->SetFallValue(pillerSpawner->GetPillerSpace());
            // hp
            statusFandP->SetcurrentHP(pillerSpawner->GetHpMax());

            if (i == 0 && j == 0) {
                statusFandP->SetcurrentHP(0);
            }

            // ================================= System ================================= //

            //------------------ Input
            // None

            //------------------ StateTransition
            ecs->getSystem<CanageStateFallSystem>()->addEntity(floor);
            ecs->getSystem<DeleteFloatingFloorEntitySystem>()->addEntity(floor);
           /* ecs->getSystem<DeleteFloorSystem>()->addEntity(floor);*/
            //------------------ Movement
            ecs->getSystem<MoveSystemByRigidBody>()->addEntity(floor);
           /* ecs->getSystem<MoveSystemByRigidBody>()->addEntity(floor);*/
            ecs->getSystem<FloorAndPillerFallSystem>()->addEntity(floor);

            /*ecs->getSystem<FloorUpdateMatrixSystem>()->addEntity(floor);*/
            ecs->getSystem<PillerUpdateMatrixSystem>()->addEntity(floor);

            //------------------ Collision
            /*  ecs->getSystem<CharacterOnCollision>()->addEntity(bom);*/
            ecs->getSystem<CollisionCheckSystem>()->addEntity(floor);
            /*ecs->getSystem<CollisionCheckSystem>()->addEntity(floor);*/
            ecs->getSystem<PillerDamageSystem>()->addEntity(floor);
            //------------------ Physics
            // None

            //------------------ Render
            ecs->getSystem<ColliderRenderingSystem>()->addEntity(floor);
            ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(floor);
            /*ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(floor);*/
        }
    }
}

void TowerPositionSet::CreateBottomFloor() {
    ECSManager* ecs = ECSManager::getInstance();
    for (int32_t j = 0; j < bottomFloorStates_->GetFloorNum(); ++j) {
        GameEntity* bottomFloor = CreateEntity<Transform, Transform, Rigidbody, ModelMeshRenderer>(
            "BFloor", Transform(), Transform(), Rigidbody(), ModelMeshRenderer());

        Transform* bottomFloorTransform      = getComponent<Transform>(bottomFloor, 0);
        Transform* bottomFloorPivotTransform = getComponent<Transform>(bottomFloor, 1);

        // X配置（ここではZ方向に並べて奥行き感）
        SetXPosition(bottomFloorPivotTransform, bottomFloorTransform,Vec3f(0.0f,0.0f,0.0f), j);

        ModelMeshRenderer* bottomFloorRender = getComponent<ModelMeshRenderer>(bottomFloor);

        if (j % 2 == 0) {
            CreateModelMeshRenderer(bottomFloorRender, bottomFloor, kApplicationResourceDirectory + "/Models/whiteFloor", "whiteFloor.obj");
        } else {
            CreateModelMeshRenderer(bottomFloorRender, bottomFloor, kApplicationResourceDirectory + "/Models/redFloor", "redFloor.obj");
        }

        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(bottomFloor);
        ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(bottomFloor);
    }
}

void TowerPositionSet::SetXPosition(Transform* baseTransform, Transform* childTransform, const Vec3f& offset, int32_t rowIndex) {
 
    float baseY = 0.0f;
    float baseZ = 0.0f;

    baseTransform->translate = Vec3f(float(rowIndex) * pillerSpawner->GetFloorSpace(), baseY, baseZ);
    baseTransform->rotate    = Quaternion::Identity();
    baseTransform->Update();

    // 奥に向けてZで並べる（またはここをYにすれば縦積み2D）
    childTransform->translate = offset;
    childTransform->rotate    = Quaternion::Identity();
    childTransform->parent    = baseTransform;
    childTransform->Update();
}

void TowerPositionSet::CostInit() {
    safeCost_   = 0;
    normalCost_ = 0;
}
