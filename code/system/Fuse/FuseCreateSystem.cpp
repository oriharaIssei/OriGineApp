#include "FuseCreateSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
#include "myRandom/MyRandom.h"

#include <Vector3.h>
// component

#include "component/Block/BlockCombinationStatus.h"
#include "component/Block/BlockStatus.h"
#include "component/Fuse/FuseStatus.h"
#include "component/Menu/MenuStatus.h"
//  system
#include "system/Fuse/FuseAdaptPosSystem.h"
#include "system/Fuse/FuseCreateSystem.h"
#include "system/Fuse/FuseDeleteSystem.h"
#include"system/Fuse/FuseChangeSystem.h"
#include <cstdint>

FuseCreateSystem::FuseCreateSystem() : ISystem(SystemType::Movement) {}
FuseCreateSystem::~FuseCreateSystem() {}

void FuseCreateSystem::Initialize() {
    isInited_ = false;
}

void FuseCreateSystem::Finalize() {
}

void FuseCreateSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    // ポーズ中は通さない
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* menuEntity                   = ecsManager->getUniqueEntity("Menu");
    GameEntity* bockCombiEntity              = ecsManager->getUniqueEntity("BlockCombination");

    if (!menuEntity || !bockCombiEntity) {
        return;
    }

    MenuStatus* menu        = getComponent<MenuStatus>(menuEntity);
    blockCombinationStatus_ = getComponent<BlockCombinationStatus>(bockCombiEntity);
    if (!menu || !blockCombinationStatus_) {
        return;
    }

    if (menu->GetIsPose()) {
        return;
    }

    BlockStatus* blockStatus = getComponent<BlockStatus>(_entity);
    Transform* transform     = getComponent<Transform>(_entity);

    if (!blockStatus || !transform) {
        return;
    }
    if (blockStatus->GetRowNum() < blockCombinationStatus_->GetConbinationMax()) {
        return;
    }

    //
    if (blockStatus->GetBlockType() == BlockType::ADVANTAGE) {
        blockCombinationStatus_->ChangeStatusAdvantageStart(blockStatus->GetRowNum(), blockStatus->GetColumnNum());
    } else {
        blockCombinationStatus_->ChangeStatusFuse(blockStatus->GetRowNum(), blockStatus->GetColumnNum());
    }

    // 導火線追加
    CreateFuse(_entity, blockStatus);
    ////導火線変更
    //ChangeFuse(_entity, blockStatus);

}

void FuseCreateSystem::CreateFuse(GameEntity* _entity, BlockStatus* blockStatus) {
    _entity;
    // 生成
    if (!blockStatus->GetIsCreateFuse() || blockStatus->GetIsCreatedFuse()) {
        return;
    }

    GameEntity* fuseCenter = CreateEntity<Transform, Rigidbody, ModelMeshRenderer, FuseStatus>("FuseCenters", Transform(), Rigidbody(), ModelMeshRenderer(), FuseStatus());
    FuseStatus* fuseStatus = getComponent<FuseStatus>(fuseCenter);
    //* model
    ModelMeshRenderer* fuseCenterModel = getComponent<ModelMeshRenderer>(fuseCenter);
    switch (blockStatus->GetFuseMode()) {
    case FuseMode::NONE:
        fuseCenterModel->setIsRender(false);
        break;

    case FuseMode::START:
        CreateModelMeshRenderer(fuseCenterModel, fuseCenter, kApplicationResourceDirectory + "/Models/Fuse", "Fuse.obj");
        fuseStatus->SetOffSetPos(Vec3f(0.0f, 0.0f, -6.0f));
        break;

    case FuseMode::CENTER:
        CreateModelMeshRenderer(fuseCenterModel, fuseCenter, kApplicationResourceDirectory + "/Models/Fuse_Long", "Fuse_Long.obj");
        fuseStatus->SetOffSetPos(Vec3f(-5.0f, 0.0f, -6.0f));
        break;

    case FuseMode::END:
        break;
    default:
        break;
    }
   
    // set
   
    fuseStatus->SetBasePosition(&blockStatus->GetBasePos());
    fuseStatus->SetIsDeath(&blockStatus->GetIsBreak());
    fuseStatus->SetFuseMode(&blockStatus->GetFuseMode());
    fuseStatus->SetMaxScale(Vec3f(7.5f, 3.5f, 2.5f));
    fuseStatus->SetStartScale(Vec3f(0.0f, 3.5f, 2.5f));
    fuseStatus->Reset();

    // 既存のエンティティ（fuse）にシステム追加処理
    ECSManager* ecs = ECSManager::getInstance();

    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(fuseCenter);
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(fuseCenter);

    ecs->getSystem<FuseAdaptPosSystem>()->addEntity(fuseCenter); 
    ecs->getSystem<FuseDeleteSystem>()->addEntity(fuseCenter);
    ecs->getSystem<FuseChangeSystem>()->addEntity(fuseCenter);

    blockStatus->SetIsCreatedFuse(true);
}

//void FuseCreateSystem::ChangeFuse(GameEntity* _entity, BlockStatus* blockStatus) {
//
// }

//// アドバンテージブロック処理
// if (blockStatus->GetBlockType() == BlockType::ADVANTAGE) {
//     blockCombinationStatus_->ChangeStatusAdvantageStart(blockStatus->GetRowNum(), blockStatus->GetColumnNum());
//     blockCombinationStatus_->ChangeStatusRightEdge(blockStatus->GetRowNum(), blockStatus->GetColumnNum());
//  //生成するよフラグ
// }

/* blockCombinationStatus_->ChangeCenterFuse(blockStatus->GetRowNum(), blockStatus->GetColumnNum());*/

//// STARTとENDブロックを探す
// BlockStatus* startBlock = nullptr;
// BlockStatus* endBlock   = nullptr;

// for (BlockStatus* block : blockCombinationStatus_->GetBlockStatusArray()) {
//     if (block->GetColumnNum() != blockStatus->GetColumnNum())
//         continue;
//     if (block->GetIsBreak())
//         continue;

//    if (block->GetFuseMode() == FuseMode::START) {
//        startBlock = block;
//    } else if (block->GetFuseMode() == FuseMode::END) {
//        endBlock = block;
//    }
//}

//// 線を生成（Transformが取得できる場合のみ）
// if (startBlock && endBlock) {
//     startBlock->GetBasePos();
//     endBlock->GetBasePos();

//    // getpos
//    Vector3 startPos = startBlock->GetBasePos();
//    Vector3 endPos   = endBlock->GetBasePos();
//    startBlock->GetRowNum();
//    endBlock->GetRowNum();

//    int32_t createNum = startBlock->GetRowNum() - endBlock->GetRowNum();

//    //　指定の数分生成する
//    for (int32_t i = 0; i < createNum; ++i) {
//        GameEntity* fuseCenter = CreateEntity<Transform, Rigidbody, ModelMeshRenderer>(
//            "FuseCenters", Transform(), Rigidbody(), ModelMeshRenderer());

//        ModelMeshRenderer* fuseCenterModel = getComponent<ModelMeshRenderer>(fuseCenter);
//        CreateModelMeshRenderer(fuseCenterModel, fuseCenter, kApplicationResourceDirectory + "/Models/Floor_Gold", "Floor_Gold.obj");
//    }
//}
