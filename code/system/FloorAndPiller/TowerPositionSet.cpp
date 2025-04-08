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
#include "component/FloorAndPillerColum/FloorAndPillerrStatus.h"
#include "component/FloorAndPillerColum/FloorAndPillerSpawner.h"
#include "component/Piller/PillerStates.h"
// system
// #include "system/FloorAndPillerColum/CreateFloorAndPillerSystem.h"
#include "system/FloorAndPiller/DeleteFloorAndPillerSystem.h"
#include "system/FloorAndPiller/FloorAndPillerFallSystem.h"
#include "system/Piller/DeletePillerSystem.h"
#include "system/Piller/PillerDamageSystem.h"
#include <cmath>
#include <numbers>

TowerPositionSet::TowerPositionSet() : ISystem(SystemType::Movement) {}
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
    pillerStates_          = getComponent<PillerStates>(_entity);
    floorAndPillerSpawner_ = getComponent<FloorAndPillerSpawner>(_entity);

    if (!bottomFloorStates_ || !floorStates_ || !pillerStates_ || !floorAndPillerSpawner_) {
        return;
    }

    floorModeCreater_.resize(bottomFloorStates_->GetFloorNum());

    for (int32_t i = 0; i < floorModeCreater_.size(); ++i) {
        floorModeCreater_[i] = getComponent<FloorModeCreater>(_entity);
        if (!floorModeCreater_[i]) {
            return;
        }
    }

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
            GameEntity* piller         = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer, PillerStates>("Piller", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer(), PillerStates());
            GameEntity* floor          = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer, FloorStates>("Floor", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer(), FloorStates());
            GameEntity* floorAndPiller = CreateEntity<Transform, Rigidbody, FloorAndPillerrStatus>("FAndP", Transform(), Rigidbody(), FloorAndPillerrStatus());

            // ================================= Componentを初期化 ================================= //

            Transform* fAndPTransform  = getComponent<Transform>(floorAndPiller);
            Transform* pillerTransform = getComponent<Transform>(piller); // 柱
            Transform* floorTransform  = getComponent<Transform>(floor); // 床

            /// ランダムで床のサイズを変える（通常がコスト1,デカイのがコスト2）Maxコスト6

            // ランダムでセーフゾーンがあるかを設定（セーフゾーンありがコスト1,セーフゾーンなしがコスト0）Maxコスト2

            // Transformの初期位置を設定
            SetPivotQuaternion(fAndPTransform, j);

            // 　親のY位置を設定
            if (i == 0) {
                fAndPTransform->translate = Vec3f(0.0f, 0.0f, 0.0f);

            } else {
                fAndPTransform->translate = Vec3f(0.0f, 0.0f + (floorAndPillerSpawner_->GetPillerSpace() * float(i)), 0.0f);
            }
            /* Vec3f(fAndPTransform->translate) + Vec3f(fAndPTransform->translate) +*/
            // 子の
            pillerTransform->translate = Vec3f(0.0f, floorAndPillerSpawner_->GetFirstPillerOffset(), Radius);
            floorTransform->translate  = Vec3f(0.0f, floorAndPillerSpawner_->GetFloorSpace(), Radius);

            SetQuaternion(fAndPTransform, pillerTransform, pillerTransform->translate);
            SetQuaternion(fAndPTransform, floorTransform, floorTransform->translate);

            // Collider
            SphereCollider* collider           = getComponent<SphereCollider>(piller);
            collider->getLocalShape()->radius_ = pillerStates_->GetCollisionSize();
            /* collider->getWorldShape()->radius_ = pillerStates_->GetCollisionSize();*/

            // MeshRenderer
            ModelMeshRenderer* pillerRender = getComponent<ModelMeshRenderer>(piller);
            ModelMeshRenderer* floorRender  = getComponent<ModelMeshRenderer>(floor);

            // Model から MeshRenderer を作成
            CreateModelMeshRenderer(pillerRender, piller, kApplicationResourceDirectory + "/Models/Piller", "Piller.obj");
            CreateModelMeshRenderer(floorRender, floor, kApplicationResourceDirectory + "/Models/whiteFloor", "whiteFloor.obj");

            // ↓ここのパラメータコピーがエディターで設定した値にならない(初期化設定した値は問題なし)

            /// States
            FloorStates* statusFloor = getComponent<FloorStates>(floor);
            statusFloor              = floorStates_;

            PillerStates* statusPiller = getComponent<PillerStates>(piller);
            statusPiller               = pillerStates_;
            statusPiller->SetCurrentHp(pillerStates_->GetHpMax());

            // row,columNum
            FloorAndPillerrStatus* statusFandP = getComponent<FloorAndPillerrStatus>(floorAndPiller);
            statusFandP->SetColumAndRow(i, j);
            statusPiller->SetColumAndRow(statusFandP->GetColumNum(), statusFandP->GetRowNum());
            // savePosを設定
            statusFandP->SetSavePos(fAndPTransform->translate[Y]);
            // 落ちるオフセットを決める
            statusFandP->SetFallValue(floorAndPillerSpawner_->GetPillerSpace());

            if (i == 0 && j == 0) {
                statusPiller->SetCurrentHp(0);
            }
            // ================================= System ================================= //

            //------------------ Input
            // None

            //------------------ StateTransition
            ecs->getSystem<DeletePillerSystem>()->addEntity(piller);
            ecs->getSystem<DeleteFloorAndPillerSystem>()->addEntity(piller);

            //------------------ Movement
            ecs->getSystem<MoveSystemByRigidBody>()->addEntity(piller);
            ecs->getSystem<MoveSystemByRigidBody>()->addEntity(floor);
            ecs->getSystem<MoveSystemByRigidBody>()->addEntity(floorAndPiller);
            ecs->getSystem<FloorAndPillerFallSystem>()->addEntity(floorAndPiller);

                //------------------ Collision
                /*  ecs->getSystem<CharacterOnCollision>()->addEntity(bom);*/
            ecs->getSystem<CollisionCheckSystem>()->addEntity(floor);
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
        GameEntity* bottomFloor = CreateEntity<Transform, Transform, Rigidbody, ModelMeshRenderer, FloorStates, FloorAndPillerSpawner, PillerStates, FloorModeCreater>("BFloor", Transform(), Transform(), Rigidbody(), ModelMeshRenderer(), FloorStates(), FloorAndPillerSpawner(), PillerStates(), FloorModeCreater());

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
        CreateModelMeshRenderer(bottomFloorRender, bottomFloor, kApplicationResourceDirectory + "/Models/whiteFloor", "whiteFloor.obj");

        /// States
        FloorStates* floorStates                     = getComponent<FloorStates>(bottomFloor); // 床
        PillerStates* pillerStates                   = getComponent<PillerStates>(bottomFloor); // 柱
        FloorAndPillerSpawner* floorAndPillerSpawner = getComponent<FloorAndPillerSpawner>(bottomFloor); // 床
        FloorModeCreater* floorModeCreater           = getComponent<FloorModeCreater>(bottomFloor); // 床

        floorStates           = floorStates_;
        pillerStates          = pillerStates_;
        floorAndPillerSpawner = floorAndPillerSpawner_;
        floorModeCreater      = floorModeCreater_[j];

        // rowNumberをセット
        floorAndPillerSpawner_->SetRowNumber(j);

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
