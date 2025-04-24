#include "GameScene.h"

/// ECS
#include "ECS/ECSManager.h"
// component
#include"Application/code/component/Player/PlayerStates.h"
#include"Application/code/component/Piller/FloatingFloorStatus.h"
#include"Application/code/component/Piller/FloatingFloorSpawner.h"
#include"Application/code/component/Bom/BomSpawner.h"
#include"Application/code/component/Bom/BomStatus.h"
#include"Application/code/component/Bom/ExplotionCollision.h"
#include"Application/code/component/Combo/ComboStatus.h"
#include "Application/code/component/Combo/ComboUIStatus.h"
#include"Application/code/component/Timer/TimerUIStatus.h"
#include"Application/code/component/Timer/TimerStatus.h"
#include"Application/code/component/Block/BlockManager.h"
#include"Application/code/component/Block/BlockStatus.h"
#include"Application/code/component/Scrap/ScrapSpawner.h"
#include"Application/code/component/Scrap/ScrapStatus.h"
#include"Application/code/component/BigBom/BigBomSpawner.h"
#include"Application/code/component/BigBom/BigBomStatus.h"
#include"Application/code/component/BigBom/BigExplotionCollision.h"
#include"Application/code/component/Score/ScoreStatus.h"
#include"Application/code/component/Score/ScoreUIStatus.h"
#include"Application/code/component/Field/FieldStatus.h"
#include"Application/code/component/EffectByBlock/EffectByBlockUIStatus.h"
#include"Application/code/component/OperateUI/OperateUIStatus.h"
#include"Application/code/component/GameEnd/GameEnd.h"
#include"Application/code/component/Button.h"
#include"Application/code/component/SceneChanger.h"
//#include"Application/code/component/Floor/FloorModeCreater.h"
//#include"Application/code/component/Floor/BottomFloorStates.h"
//#include"Application/code/component/Floor/FloorStates.h"

//system
#include"Application/code/system/Player/PlayerInput.h"
#include"Application/code/system/Player/PlayerMoveSystem.h"
#include"Application/code/system/Bom/PutBomSystem.h"
#include"Application/code/system/Bom/BomExplotionSystem.h"
#include"Application/code/system/Bom/DeleteBomSystem.h"
#include"Application/code/system/Bom/DeleteExplotionCollision.h"
#include"Application/code/system/Player/PlayerFollowCameraSystem.h"
#include"Application/code/system/Player/PlayerCreateBigBomSystem.h"
#include"Application/code/system/Floor/DeleteFloorSystem.h"
#include"Application/code/system/Floor/FloorUpdateMatrixSystem.h"
#include"Application/code/system/Combo/ComboUIScrollSystem.h"
#include"Application/code/system/Combo/ComboSystem.h"
#include "Application/code/system/Timer/TimerDecrementSystem.h"
#include"Application/code/system/Timer/TimerUIScrollSystem.h"
#include"Application/code/system/FloatingFloor/CreateFloorSystem.h"
#include"Application/code/system/FloatingFloor/FloatingFloorFallSystem.h"
#include"Application/code/system/FloatingFloor/DeleteFloatingFloorSystem.h"
#include"Application/code/system/FloatingFloor/FloatingFloorDamageSystem.h"
#include"Application/code/system/FloatingFloor/CanageStateFallSystem.h"
#include"Application/code/system/Matrix/UpdateMatrixSystem.h"
#include"Application/code/system/Block/BlockSpawnSystem.h"
#include"Application/code/system/Block/BlockMoveSystem.h"
#include"Application/code/system/Block/DeleteBlockSystem.h"
#include"Application/code/system/Block/MoveTenpoSystem.h"
#include"Application/code/system/Bom/BomCollisionExSystem.h"
#include"Application/code/system/Block/BreakBlockSystem.h"
#include"Application/code/system/Block/BlockExBomCollision.h"
#include"Application/code/system/scrap/ScrapFallSystem.h"
#include"Application/code/system/scrap/ScrapDeleteSystem.h"
#include"Application/code/system/scrap/ScrapToPlayerCollisionSystem.h"
#include"Application/code/system/FloatingFloor/FloatingFloorRevivalSystem.h"
#include"Application/code/system/Block/BlockFloorCollision.h"
#include"Application/code/system/Score/ScoreIncrementSystem.h"
#include"Application/code/system/Score/ScoreUIScrollSystem.h"
#include"Application/code/system/BigBom/BigBomCollisionExSystem.h"
#include"Application/code/system/BigBom/DeleteBigExplotionCollision.h"
#include"Application/code/system/BigBom/BigBomInputSystem.h"
#include"Application/code/system/BigBom/BigBomLaunchSystem.h"
#include"Application/code/system/BigBom/DeleteBigBomSystem.h"
#include"Application/code/system/EffectByBlock/EffectByBlockDrawSystem.h"
#include"Application/code/system/EffectByBlock/EffectByBlockDeleteSystem.h"
#include"Application/code/system/OperateUI/OperateUISystem.h"
#include"Application/code/system/GameEnd/GameEndSystem.h"
#include"Application/code/system/ButtonInputSystem.h"
#include"Application/code/system/ChangeSceneByButton.h"
//#include"Application/code/system/GameEnd/"

GameScene::GameScene()
    : IScene("Game") {}

GameScene::~GameScene() {}

void GameScene::registerComponents() {
    IScene::registerComponents();

    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerComponent<PlayerStates>();
    ecsManager->registerComponent<FloatingFloorSpawner>();
    ecsManager->registerComponent<BomSpawner>();
    ecsManager->registerComponent<BomStatus>();
    ecsManager->registerComponent<ExplotionCollision>();
    ecsManager->registerComponent<FloatingFloorStatus>();
    ecsManager->registerComponent<ComboStatus>();
    ecsManager->registerComponent<ComboUIStatus>();
    ecsManager->registerComponent<TimerStatus>();
    ecsManager->registerComponent<TimerUIStatus>();
    ecsManager->registerComponent<BlockManager>();
    ecsManager->registerComponent<BlockStatus>();
    ecsManager->registerComponent<ScrapSpawner>();
    ecsManager->registerComponent<ScrapStatus>();
    ecsManager->registerComponent<BigBomSpawner>();
    ecsManager->registerComponent<BigBomStatus>();
    ecsManager->registerComponent<BigExplotionCollision>();
    ecsManager->registerComponent<ScoreStatus>();
    ecsManager->registerComponent<ScoreUIStatus>();
    ecsManager->registerComponent<FieldStatus>();
    ecsManager->registerComponent<EffectByBlockUIStatus>();
    ecsManager->registerComponent<OperateUIStatus>();
    ecsManager->registerComponent<GameEnd>();
    ecsManager->registerComponent<Button>();
    ecsManager->registerComponent<SceneChanger>();

    /*ecsManager->registerComponent<BottomFloorStates>();
    ecsManager->registerComponent<FloorStates>();
    ecsManager->registerComponent<FloorModeCreater>();*/
}

void GameScene::registerSystems() {
    IScene::registerSystems();

    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerSystem<PlayerInputSystem>();
    ecsManager->registerSystem<PlayerMoveSystem>();
    ecsManager->registerSystem<PutBomSystem>();
    ecsManager->registerSystem<CreateFloorSystem>();
    ecsManager->registerSystem<BomExplotionSystem>();
    ecsManager->registerSystem<DeleteBomSystem>();
    ecsManager->registerSystem<FloatingFloorDamageSystem>();
    ecsManager->registerSystem<CanageStateFallSystem>();
    ecsManager->registerSystem<DeleteExplotionCollision>();
    ecsManager->registerSystem<PlayerFollowCameraSystem>();
    ecsManager->registerSystem<DeleteFloatingFloorSystem>();
    ecsManager->registerSystem<FloatingFloorFallSystem>();
    ecsManager->registerSystem<UpdateMatrixSystem>();
    ecsManager->registerSystem<DeleteFloorSystem>();
    ecsManager->registerSystem<FloorUpdateMatrixSystem>();
    ecsManager->registerSystem<ComboUIScrollSystem>();
    ecsManager->registerSystem<ComboSystem>();
    ecsManager->registerSystem<TimerDecrementSystem>();
    ecsManager->registerSystem<TimerUIScrollSystem>();
    ecsManager->registerSystem<BlockSpawnSystem>();
    ecsManager->registerSystem<BlockMoveSystem>();
    ecsManager->registerSystem<DeleteBlockSystem>();
    ecsManager->registerSystem<MoveTenpoSystem>();
    ecsManager->registerSystem<BomCollisionExSystem>();
    ecsManager->registerSystem<BreakBlockSystem>();
    ecsManager->registerSystem<BlockExBomCollision>();
    ecsManager->registerSystem<ScrapFallSystem>();
    ecsManager->registerSystem<ScrapDeleteSystem>();
    ecsManager->registerSystem<ScrapToPlayerCollisionSystem>();
    ecsManager->registerSystem<PlayerCreateBigBomSystem>();
    ecsManager->registerSystem<BigBomInputSystem>();
    ecsManager->registerSystem<BigBomLaunchSystem>();
    ecsManager->registerSystem<FloatingFloorRevivalSystem>();
    ecsManager->registerSystem<BlockFloorCollision>();
    ecsManager->registerSystem<ScoreIncrementSystem>();
    ecsManager->registerSystem<ScoreUIScrollSystem>();
    ecsManager->registerSystem<BigBomCollisionExSystem>();
    ecsManager->registerSystem<DeleteBigBomSystem>();
    ecsManager->registerSystem<DeleteBigExplotionCollision>();
    ecsManager->registerSystem<EffectByBlockDrawSystem>();
    ecsManager->registerSystem<EffectByBlockDeleteSystem>();
    ecsManager->registerSystem<OperateUISystem>();
    ecsManager->registerSystem<GameEndSystem>();
    ecsManager->registerSystem<ButtonInputSystem>();
    ecsManager->registerSystem<ChangeSceneByButton>();
    ecsManager->SortPriorityOrderSystems();
}
