#include "BottomFloorPositionSet.h"

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
#include "component/FloorAndPillerColum/FloorAndPillerSpawner.h"
#include "component/Piller/PillerStates.h"
// system
#include "system/FloorAndPillerColum/CreateFloorAndPillerSystem.h"
#include <cmath>
#include <numbers>

BottomFloorPositionSet::BottomFloorPositionSet() : ISystem(SystemType::Movement) {}
BottomFloorPositionSet::~BottomFloorPositionSet() {}

void BottomFloorPositionSet::Initialize() {
    isInited_ = false;
}

void BottomFloorPositionSet::Finalize() {
}

void BottomFloorPositionSet::UpdateEntity(GameEntity* _entity) {
    if (!_entity || isInited_) {
        return;
    }

    bottomFloorStates_ = getComponent<BottomFloorStates>(_entity);

    floorStates_           = getComponent<FloorStates>(_entity);
    pillerStates_          = getComponent<PillerStates>(_entity);
    floorAndPillerSpawner_ = getComponent<FloorAndPillerSpawner>(_entity);

   floorModeCreater_.resize(bottomFloorStates_->GetFloorNum());

    for (int32_t i = 0; i < floorModeCreater_.size(); ++i) {
        floorModeCreater_[i] = getComponent<FloorModeCreater>(_entity);
        if (!floorModeCreater_[i]) {
            return;
        }
    }

    if (!bottomFloorStates_ || !floorStates_ || !pillerStates_ || !floorAndPillerSpawner_) {
        return;
    }

    // 床生成
    CreateBottomFloor();

    isInited_ = true;
}

void BottomFloorPositionSet::CreateBottomFloor() {

    for (int32_t i = 0; i < bottomFloorStates_->GetFloorNum(); ++i) {

        // ================================= Bullet Entityを 生成 ================================= //
        GameEntity* bottomFloor = CreateEntity<Transform, Transform, Rigidbody, ModelMeshRenderer, FloorStates, FloorAndPillerSpawner, PillerStates, FloorModeCreater>("BFloor", Transform(), Transform(), Rigidbody(), ModelMeshRenderer(), FloorStates(), FloorAndPillerSpawner(), PillerStates(), FloorModeCreater());

        // ================================= Componentを初期化 ================================= //

        // Transform

        Transform* floorTransform      = getComponent<Transform>(bottomFloor, 0); // 床
        Transform* floorPivotTransform = getComponent<Transform>(bottomFloor, 1); // 床

        ///============================================================
        // ピボットをワールド座標の(0,0,0)に設定、ペアレント
        ///============================================================
        floorPivotTransform->translate = Vec3f(0.0f, 0.0f, 0.0f);
        floorPivotTransform->rotate    = Quaternion::Identity();
        floorTransform->rotate         = Quaternion::Identity();
        floorTransform->parent         = floorPivotTransform;

        ///============================================================
        // Transformの初期位置を設定
        ///============================================================
        float moveRadius          = bottomFloorStates_->GetFieldRadius();
        floorTransform->translate = Vec3f(0.0f, 0.0f, moveRadius); // X軸上に配置

        ///============================================================
        // Y軸回転のQuaternionを作成
        ///============================================================
        Quaternion rotation =
            Quaternion::RotateAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), bottomFloorStates_->GetSettingAngle() * float(i));
        floorPivotTransform->rotate *= rotation;

        floorPivotTransform->Update();
        floorTransform->Update();

        // MeshRenderer
        ModelMeshRenderer* floorRender = getComponent<ModelMeshRenderer>(bottomFloor);

        // Model から MeshRenderer を作成
        CreateModelMeshRenderer(floorRender, bottomFloor, kApplicationResourceDirectory + "/Models/whiteFloor", "whiteFloor.obj");

        /// States
        FloorStates* floorStates                     = getComponent<FloorStates>(bottomFloor); // 床
        PillerStates* pillerStates                   = getComponent<PillerStates>(bottomFloor); // 柱
        FloorAndPillerSpawner* floorAndPillerSpawner = getComponent<FloorAndPillerSpawner>(bottomFloor); // 床
        FloorModeCreater* floorModeCreater           = getComponent<FloorModeCreater>(bottomFloor); // 床

        floorStates           = floorStates_;
        pillerStates          = pillerStates_;
        floorAndPillerSpawner = floorAndPillerSpawner_;
        floorModeCreater      = floorModeCreater_[i];

        // ================================= System ================================= //
        ECSManager* ecs = ECSManager::getInstance();
        //------------------ Input
        // None

        //------------------ StateTransition
        /*ecs->getSystem<DeleteCharacterEntitySystem>()->addEntity(bullet);*/

        //------------------ Movement
        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(bottomFloor);
        ecs->getSystem<CreateFloorAndPillerSystem>()->addEntity(bottomFloor);

        //------------------ Collision
        /*  ecs->getSystem<CharacterOnCollision>()->addEntity(bom);*/

        //------------------ Physics
        // None

        //------------------ Render

        ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(bottomFloor);
    }
}
