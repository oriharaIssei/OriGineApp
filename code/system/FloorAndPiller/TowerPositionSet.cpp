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
#include "component/Floor/FloorModeCreater.h"
#include "component/Floor/FloorStates.h"
#include "component/Piller/PillerStatus.h"
#include "component/Piller/PillerSpawner.h"
//#include "component/Piller/PillerStates.h"
// system
// #include "system/FloorAndPillerColum/CreateFloorAndPillerSystem.h"
#include "system/Piller/DeletePillerEntitySystem.h"
#include "system/FloorAndPiller/FloorAndPillerFallSystem.h"
#include "system/Piller/CanageStateFallSystem.h"
#include "system/Piller/PillerDamageSystem.h"
#include "system/Piller/pillerUpdateMatrixSystem.h"
#include "system/Floor/FloorUpdateMatrixSystem.h"
#include"system/Floor/DeleteFloorSystem.h"

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

    floorStates_           = getComponent<FloorStates>(_entity);
   /* pillerStates_          = getComponent<PillerStates>(_entity);*/
    floorAndPillerSpawner_ = getComponent<FloorAndPillerSpawner>(_entity);

    if (!bottomFloorStates_ || !floorStates_  || !floorAndPillerSpawner_) {
        return;
    }

    /*floorModeCreater_.resize(bottomFloorStates_->GetFloorNum());

    for (int32_t i = 0; i < floorModeCreater_.size(); ++i) {
        floorModeCreater_[i] = getComponent<FloorModeCreater>(_entity);
        if (!floorModeCreater_[i]) {
            return;
        }
    }*/

    float moveRadius = bottomFloorStates_->GetFieldRadius();
    // 床生成
    CreateBottomFloor(-moveRadius);
    CreateTower(-moveRadius);
    isInited_ = true;
}

void TowerPositionSet::CreateTower(const float& Radius) {
    ECSManager* ecs = ECSManager::getInstance();

    for (int32_t i = 0; i < floorAndPillerSpawner_->GetColumNumMax(); ++i) {
        CostInit();
        for (int32_t j = 0; j < bottomFloorStates_->GetFloorNum(); ++j) {

            // ================================= Bullet Entityを 生成 ================================= //
            GameEntity* piller = CreateEntity<Transform, Transform, SphereCollider, AABBCollider, Rigidbody, ModelMeshRenderer, FloorAndPillerrStatus>("Piller", Transform(), Transform(), SphereCollider(), AABBCollider(), Rigidbody(), ModelMeshRenderer(), FloorAndPillerrStatus());
            GameEntity* floor  = CreateEntity<Transform, Rigidbody, ModelMeshRenderer, FloorStates>("Floor", Transform(), Rigidbody(), ModelMeshRenderer(), FloorStates());
            /*GameEntity* floorAndPiller = CreateEntity<Transform, Rigidbody, FloorAndPillerrStatus>("FAndP", Transform(), Rigidbody(), FloorAndPillerrStatus());*/
            // ================================= Componentを初期化 ================================= //

           
            Transform* pillerTransform = getComponent<Transform>(piller); // 柱
            Transform* pillerBaseTransform  = getComponent<Transform>(piller,1);
            Transform* floorTransform  = getComponent<Transform>(floor); // 床

            /// ランダムで床のサイズを変える（通常がコスト1,デカイのがコスト2）Maxコスト6

            // ランダムでセーフゾーンがあるかを設定（セーフゾーンありがコスト1,セーフゾーンなしがコスト0）Maxコスト2

            // Transformの初期位置を設定
            SetPivotQuaternion(pillerBaseTransform, j);

            // 　親のY位置を設定
            if (i == 0) {
                pillerBaseTransform->translate = Vec3f(0.0f, 0.0f, 0.0f);

            } else {
                pillerBaseTransform->translate = Vec3f(0.0f, 0.0f + (floorAndPillerSpawner_->GetPillerSpace() * float(i)), 0.0f);
            }
            
            // 子の
            pillerTransform->translate = Vec3f(0.0f, floorAndPillerSpawner_->GetFirstPillerOffset(), Radius);
            floorTransform->translate  = Vec3f(0.0f, floorAndPillerSpawner_->GetFloorSpace(), Radius);

            SetQuaternion(pillerBaseTransform, pillerTransform, pillerTransform->translate);
            SetQuaternion(pillerBaseTransform, floorTransform, floorTransform->translate);

            //* Collider
            // Sphere
            SphereCollider* collider           = getComponent<SphereCollider>(piller);
            collider->getLocalShapePtr()->radius_ = floorAndPillerSpawner_->GetCollisionSize();
            // AABB
            AABBCollider* aabbCollider = getComponent<AABBCollider>(piller);
            aabbCollider->getLocalShape().min_
            aabbCollider->getLocalShape().max_

            // MeshRenderer
            ModelMeshRenderer* pillerRender = getComponent<ModelMeshRenderer>(piller);
            ModelMeshRenderer* floorRender  = getComponent<ModelMeshRenderer>(floor);

            // Model から MeshRenderer を作成
            CreateModelMeshRenderer(pillerRender, piller, kApplicationResourceDirectory + "/Models/Piller", "Piller.obj");
            if (j %2== 0) {
                CreateModelMeshRenderer(floorRender, floor, kApplicationResourceDirectory + "/Models/whiteFloor", "whiteFloor.obj");
            } else {
                CreateModelMeshRenderer(floorRender, floor, kApplicationResourceDirectory + "/Models/redFloor", "redFloor.obj");
            }
            // ↓ここのパラメータコピーがエディターで設定した値にならない(初期化設定した値は問題なし)

            /// States
            FloorStates* statusFloor = getComponent<FloorStates>(floor);
            statusFloor              = floorStates_;

            // row,columNum
            FloorAndPillerrStatus* statusFandP = getComponent<FloorAndPillerrStatus>(piller);
            statusFandP->SetColumAndRow(i, j);
  
            // savePosを設定
            statusFandP->SetSavePos(pillerBaseTransform->translate[Y]);
            // 落ちるオフセットを決める
            statusFandP->SetFallValue(floorAndPillerSpawner_->GetPillerSpace());
            //hp
            statusFandP->SetcurrentHP(floorAndPillerSpawner_->GetHpMax());

            if (i == 0 && j == 0) {
                statusFandP->SetcurrentHP(0);
            }

            // ================================= System ================================= //

            //------------------ Input
            // None

            //------------------ StateTransition
            ecs->getSystem<CanageStateFallSystem>()->addEntity(piller);
            ecs->getSystem<DeletePillerEntitySystem>()->addEntity(piller);
            ecs->getSystem<DeleteFloorSystem>()->addEntity(floor);
            //------------------ Movement
            ecs->getSystem<MoveSystemByRigidBody>()->addEntity(piller);
            ecs->getSystem<MoveSystemByRigidBody>()->addEntity(floor);
            ecs->getSystem<FloorAndPillerFallSystem>()->addEntity(piller);

            ecs->getSystem<FloorUpdateMatrixSystem>()->addEntity(floor);
            ecs->getSystem<PillerUpdateMatrixSystem>()->addEntity(piller);

                //------------------ Collision
            /*  ecs->getSystem<CharacterOnCollision>()->addEntity(bom);*/
            ecs->getSystem<CollisionCheckSystem>()->addEntity(piller);
            ecs->getSystem<PillerDamageSystem>()->addEntity(piller);
            //------------------ Physics
            // None

            //------------------ Render
            ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(piller);
            ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(floor);
        }
    }
}

void TowerPositionSet::CreateBottomFloor(const float& Radius) {
    ECSManager* ecs = ECSManager::getInstance();
    for (int32_t j = 0; j < bottomFloorStates_->GetFloorNum(); ++j) {
        // ================================= Bullet Entityを 生成 ================================= //
        GameEntity* bottomFloor = CreateEntity<Transform, Transform, Rigidbody, ModelMeshRenderer>("BFloor", Transform(), Transform(), Rigidbody(), ModelMeshRenderer());

        // ================================= Componentを初期化 ================================= //

        // Transform

        Transform* bottomFloorTransform      = getComponent<Transform>(bottomFloor, 0); // 床
        Transform* bottomFloorPivotTransform = getComponent<Transform>(bottomFloor, 1); // 床

        // Transformの初期位置を設定
        SetPivotQuaternion(bottomFloorPivotTransform, j);
        SetQuaternion(bottomFloorPivotTransform, bottomFloorTransform, Vec3f(0.0f, 0.0f, Radius));

        // MeshRenderer
        ModelMeshRenderer* bottomFloorRender = getComponent<ModelMeshRenderer>(bottomFloor);

        // Model から MeshRenderer を作成
        if (j % 2 == 0) {
            CreateModelMeshRenderer(bottomFloorRender, bottomFloor, kApplicationResourceDirectory + "/Models/whiteFloor", "whiteFloor.obj");
        } else {
            CreateModelMeshRenderer(bottomFloorRender, bottomFloor, kApplicationResourceDirectory + "/Models/redFloor", "redFloor.obj");
        }
      

        // rowNumberをセット
       /* floorAndPillerSpawner_->SetRowNumber(j);*/

        // ================================= System ================================= //

        //------------------ Input
        // None

        //------------------ StateTransition
        /*ecs->getSystem<DeleteCharacterEntitySystem>()->addEntity(bullet);*/

        //------------------ Movement
        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(bottomFloor);
        /*  ecs->getSystem<CreateFloorAndPillerSystem>()->addEntity(bottomFloor);*/

        //------------------ Collision
        /*  ecs->getSystem<CharacterOnCollision>()->addEntity(bom);*/

        //------------------ Physics
        // None

        //------------------ Render

        ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(bottomFloor);
    }
}

void TowerPositionSet::SetPivotQuaternion(Transform* pivotTransform, const int32_t& index) {
    ///============================================================
    // ピボットをワールド座標の(0,0,0)に設定、ペアレント
    ///============================================================
    pivotTransform->translate = Vec3f(0.0f, 0.0f, 0.0f);
    pivotTransform->rotate    = Quaternion::Identity();

    ///============================================================
    // Y軸回転のQuaternionを作成
    ///============================================================
    Quaternion rotation =
        Quaternion::RotateAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), bottomFloorStates_->GetSettingAngle() * float(index));
    pivotTransform->rotate *= rotation;

    pivotTransform->Update();
}

void TowerPositionSet::SetQuaternion(Transform* pivotTransform, Transform* Transform, const Vec3f& offset) {

    Transform->rotate = Quaternion::Identity();
    Transform->parent = pivotTransform;

    ///============================================================
    // Transformの初期位置を設定
    ///============================================================

    Transform->translate = offset;

    Transform->Update();
}

void TowerPositionSet::CostInit() {
    safeCost_   = 0;
    normalCost_ = 0;
}
