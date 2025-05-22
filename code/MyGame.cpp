#include "MyGame.h"

/// engine

// ECS
// component

// system

// engine include
#define ENGINE_INCLUDE
#define ENGINE_ECS
#include <EngineInclude.h>

// scene
#include "iScene/IScene.h"
#include "sceneManager/SceneManager.h"

/// lib
#include "globalVariables/GlobalVariables.h"
#include "logger/Logger.h"

#ifdef _DEBUG
/// editor
#include "ECSEditor.h"
#include "module/editor/EditorGroup.h"
/// debugger
#include "ECSDebugger.h"
#include "module/debugger/DebuggerGroup.h"
#endif // DEBUG

#include "Application/code/component/BigBom/BigBomSpawner.h"
#include "Application/code/component/BigBom/BigBomStatus.h"
#include "Application/code/component/BigBom/BigExplotionCollision.h"
#include "Application/code/component/Block/BlockManager.h"
#include "Application/code/component/Block/BlockStatus.h"
#include "Application/code/component/Bom/BomSpawner.h"
#include "Application/code/component/Bom/BomStatus.h"
#include "Application/code/component/Bom/ExplotionCollision.h"
#include "Application/code/component/Button.h"
#include "Application/code/component/Combo/ComboStatus.h"
#include "Application/code/component/Combo/ComboUIStatus.h"
#include "Application/code/component/EffectByBlock/EffectByBlockSpawner.h"
#include "Application/code/component/EffectByBlock/EffectByBlockUIStatus.h"
#include "Application/code/component/Field/FieldStatus.h"
#include "Application/code/component/LevelUPUI/LevelUIParentStatus.h"
#include "Application/code/component/LevelUPUI/LevelUIStatus.h"
#include "Application/code/component/Menu/MenuStatus.h"
#include "Application/code/component/Menu/TutorialArrowStatus.h"
#include "Application/code/component/Menu/TutorialMenuParentStatus.h"
#include "Application/code/component/Menu/TutorialMenuStatus.h"
#include "Application/code/component/OperateUI/OperateUIStatus.h"
#include "Application/code/component/Piller/FloatingFloorSpawner.h"
#include "Application/code/component/Piller/FloatingFloorStatus.h"
#include "Application/code/component/Player/PlayerStates.h"
#include "Application/code/component/SceneChanger.h"
#include "Application/code/component/SceneChanger/SceneChangerStatus.h"
#include "Application/code/component/SceneTransition/SceneTransition.h"
#include "Application/code/component/Score/ScoreStatus.h"
#include "Application/code/component/Score/ScoreUIStatus.h"
#include "Application/code/component/Scrap/ScrapSpawner.h"
#include "Application/code/component/Scrap/ScrapStatus.h"
#include "Application/code/component/Timer/TimerStatus.h"
#include "Application/code/component/Timer/TimerUIStatus.h"
#include "component/Block/BlockCombinationStatus.h"
#include "component/effect/TextureEffectParam.h"
#include "component/FloorUI/FloorUIController.h"
#include "component/FloorUI/FloorUIStatus.h"
#include "component/GameEndUI/GameEndUIStatus.h"
#include "component/ResultUI/ResultFontStatus.h"
#include "component/ResultUI/ResultUIkeyStatus.h"
#include "component/ResultUI/ResultUIParentStatus.h"
#include "component/ResultUI/ResultUIRankStatus.h"
#include "component/ResultUI/ResultUIScoreStatus.h"
#include "component/Timer/TimerAnimationStatus.h"
#include"component/effect/post/VignetteParam.h"
#include"component/Piller/FloatingFloorAnimationStatus.h"
#include"component/GameCamera/CameraRenditionStatus.h"

// system
#include "Application/code/system/BigBom/BigBomCollisionExSystem.h"
#include "Application/code/system/BigBom/BigBomInputSystem.h"
#include "Application/code/system/BigBom/BigBomLaunchSystem.h"
#include "Application/code/system/BigBom/DeleteBigBomSystem.h"
#include "Application/code/system/BigBom/DeleteBigExplotionCollision.h"
#include "Application/code/system/Block/BlockColorChangeSystem.h"
#include "Application/code/system/Block/BlockExBomCollision.h"
#include "Application/code/system/Block/BlockFloorCollision.h"
#include "Application/code/system/Block/BlockMoveSystem.h"
#include "Application/code/system/Block/BlockSpawnSystem.h"
#include "Application/code/system/Block/BreakBlockSystem.h"
#include "Application/code/system/Block/DeleteBlockSystem.h"
#include "Application/code/system/Block/MoveTenpoSystem.h"
#include "Application/code/system/Bom/BomCollisionExSystem.h"
#include "Application/code/system/Bom/BomExplotionSystem.h"
#include "Application/code/system/Bom/DeleteBomSystem.h"
#include "Application/code/system/Bom/DeleteExplotionCollision.h"
#include "Application/code/system/Bom/PutBomSystem.h"
#include "Application/code/system/ButtonInputSystem.h"
#include "Application/code/system/ChangeSceneByButton.h"
#include "Application/code/system/Combo/ComboSystem.h"
#include "Application/code/system/Combo/ComboUIScrollSystem.h"
#include "Application/code/system/EffectByBlock/EffectByBlockDeleteSystem.h"
#include "Application/code/system/EffectByBlock/EffectByBlockDrawSystem.h"
#include "Application/code/system/FloatingFloor/CanageStateFallSystem.h"
#include "Application/code/system/FloatingFloor/CreateFloorSystem.h"
#include "Application/code/system/FloatingFloor/DeleteFloatingFloorSystem.h"
#include "Application/code/system/FloatingFloor/FloatingFloorDamageSystem.h"
#include "Application/code/system/FloatingFloor/FloatingFloorFallSystem.h"
#include "Application/code/system/FloatingFloor/FloatingFloorRevivalSystem.h"
#include "Application/code/system/Floor/DeleteFloorSystem.h"
#include "Application/code/system/Floor/FloorUpdateMatrixSystem.h"
#include "Application/code/system/LeverlUP/LevelUIAdaptSystem.h"
#include "Application/code/system/LeverlUP/LevelUIAnimationSystem.h"
#include "Application/code/system/LeverlUP/LevelUIInitSystem.h"
#include "Application/code/system/LeverlUP/LevelUIParentInitSystem.h"
#include "Application/code/system/Matrix/UpdateMatrixSystem.h"
#include "Application/code/system/Menu/MenuArrowSystem.h"
#include "Application/code/system/Menu/MenuFontSystem.h"
#include "Application/code/system/Menu/MenuSystem.h"
#include "Application/code/system/Menu/MenutextSystem.h"
#include "Application/code/system/Menu/TutorialArrowSystem.h"
#include "Application/code/system/Menu/TutorialMenuParentSystem.h"
#include "Application/code/system/Menu/TutorialMenuSystem.h"
#include "Application/code/system/OperateUI/OperateUISystem.h"
#include "Application/code/system/Player/PlayerCreateBigBomSystem.h"
#include "Application/code/system/Player/PlayerFollowCameraSystem.h"
#include "Application/code/system/Player/PlayerInput.h"
#include "Application/code/system/Player/PlayerMoveSystem.h"
#include "Application/code/system/SceneChanger/SceneChangerSystem.h"
#include "Application/code/system/SceneTransitionSystem/SceneTransitionSystem.h"
#include "Application/code/system/Score/ScoreIncrementSystem.h"
#include "Application/code/system/Score/ScoreUIScrollSystem.h"
#include "Application/code/system/scrap/ScrapDeleteSystem.h"
#include "Application/code/system/scrap/ScrapFallSystem.h"
#include "Application/code/system/scrap/ScrapToPlayerCollisionSystem.h"
#include "Application/code/system/Timer/TimerDecrementSystem.h"
#include "Application/code/system/Timer/TimerUIScrollSystem.h"
#include "system/Block/DeleteBlockForAdvantageSystem.h"
#include "system/FloatingFloor/CheckIsUnderPlayer.h"
#include "system/FloorUI/FloorUISystem.h"
#include "system/GameEndUI/GameEndUISystem.h"
#include "system/ResultUI/ResultFontSystem.h"
#include "system/ResultUI/ResultKeySystem.h"
#include "system/ResultUI/ResultRankSystem.h"
#include "system/ResultUI/ResultScoreAdaptSystem.h"
#include "system/ResultUI/ResultUIParentSystem.h"
#include "system/Timer/AdapAnimationtBackTimerSystem.h"
#include "system/Timer/TimeAdaptAinmationSystem.h"
#include "system/Timer/TimerAnimationSystem.h"
#include "system/UpdateButtonColorByState.h"
#include "system/UsingCameraSetSystem.h"
#include"system/Score/PlusScoreAdaptUISystem.h"
#include"system/Score/PlusScoreIconSystem.h"
#include"system/postRender/VignetteEffect.h"
#include"system/Timer/TimerAdaptVignetSystem.h"
#include"system/FloatingFloor/FloatingFloorAnimationSystem.h"
#include"system/GameCamera/CameraShakeSystem.h"
#include"system/Block/BlockAdaptTextureSystem.h"
#include"system/Block/BlockApearSystem.h"

MyGame::MyGame() {}

MyGame::~MyGame() {}

void MyGame::Initialize() {
    ///=================================================================================================
    // Game のための 初期化
    ///=================================================================================================
    variables_ = GlobalVariables::getInstance();

    engine_       = Engine::getInstance();
    sceneManager_ = SceneManager::getInstance();

    variables_->LoadAllFile();
    engine_->Initialize();
    sceneManager_->Initialize();

    RegisterUsingComponents();
    RegisterUsingSystems();

    sceneManager_->sceneChange2StartupScene();

#ifdef _DEBUG
    ///=================================================================================================
    // Editor の初期化
    ///=================================================================================================
    {
        EditorGroup* editorGroup             = EditorGroup::getInstance();
        std::unique_ptr<ECSEditor> ecsEditor = std::make_unique<ECSEditor>();
        editorGroup->addEditor(std::move(ecsEditor));
        std::unique_ptr<GlobalVariablesEditor> globalVariables = std::make_unique<GlobalVariablesEditor>();
        editorGroup->addEditor(std::move(globalVariables));

        // gorup Initialize
        editorGroup->Initialize();
    }

    ///=================================================================================================
    // Debugger の初期化
    ///=================================================================================================
    {
        DebuggerGroup* debuggerGroup = DebuggerGroup::getInstance();

        std::unique_ptr<EntityDebugger> ecsDebugger = std::make_unique<EntityDebugger>();
        debuggerGroup->addDebugger(std::move(ecsDebugger));

        std::unique_ptr<SystemDebugger> systemDebugger = std::make_unique<SystemDebugger>();
        debuggerGroup->addDebugger(std::move(systemDebugger));

        std::unique_ptr<GuiLogger> loggerDebugger = std::make_unique<GuiLogger>();
        debuggerGroup->addDebugger(std::move(loggerDebugger));

        // gorup Initialize
        debuggerGroup->Initialize();
    }

#endif // DEBUG
}

void MyGame::Finalize() {
#ifdef _DEBUG
    EditorGroup* editorGroup     = EditorGroup::getInstance();
    DebuggerGroup* debuggerGroup = DebuggerGroup::getInstance();
    editorGroup->Finalize();
    debuggerGroup->Finalize();
#endif // _DEBUG

    SceneFinalize();

    sceneManager_->Finalize();
    engine_->Finalize();
}

void MyGame::Run() {
    while (true) {
        if (engine_->ProcessMessage()) {
            break;
        }
        engine_->BeginFrame();

#ifdef _DEBUG
        sceneManager_->DebugUpdate();
#endif // DEBUG

        sceneManager_->Update();

        engine_->EndFrame();
    }
}

void MyGame::RegisterUsingComponents() {
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
    ecsManager->registerComponent<EffectByBlockSpawner>();
    ecsManager->registerComponent<SceneChangerStatus>();
    ecsManager->registerComponent<Button>();
    ecsManager->registerComponent<LevelUIStatus>();
    ecsManager->registerComponent<LevelUIParentStatus>();
    ecsManager->registerComponent<SceneChanger>();
    ecsManager->registerComponent<TutorialMenuParentStatus>();
    ecsManager->registerComponent<TutorialMenuStatus>();
    ecsManager->registerComponent<MenuStatus>();
    ecsManager->registerComponent<TutorialArrowStatus>();
    ecsManager->registerComponent<SceneTransition>();
    ecsManager->registerComponent<ResultUIParentStatus>();
    ecsManager->registerComponent<ResultUIRankStatus>();
    ecsManager->registerComponent<ResultUIScoreStatus>();
    ecsManager->registerComponent<ResultFontStatus>();
    ecsManager->registerComponent<ResultUIkeyStatus>();
    ecsManager->registerComponent<FloorUIController>();
    ecsManager->registerComponent<FloorUIStatus>();
    ecsManager->registerComponent<GameEndUIStatus>();
    ecsManager->registerComponent<TimerAnimationStatus>();
    ecsManager->registerComponent<BlockCombinationStatus>();
    ecsManager->registerComponent<VignetteParam>();
    ecsManager->registerComponent<FloatingFloorAnimationStatus>();
    ecsManager->registerComponent<CameraRenditionStatus>();

    ecsManager->registerComponent<Transform>();
    ecsManager->registerComponent<CameraTransform>();

    ecsManager->registerComponent<DirectionalLight>();
    ecsManager->registerComponent<PointLight>();
    ecsManager->registerComponent<SpotLight>();

    ecsManager->registerComponent<Rigidbody>();

    ecsManager->registerComponent<AABBCollider>();
    ecsManager->registerComponent<SphereCollider>();

    ecsManager->registerComponent<Emitter>();

    ecsManager->registerComponent<Audio>();

    ecsManager->registerComponent<ModelNodeAnimation>();
    ecsManager->registerComponent<PrimitiveNodeAnimation>();

    ecsManager->registerComponent<ModelMeshRenderer>();
    ecsManager->registerComponent<PlaneRenderer>();
    ecsManager->registerComponent<SpriteRenderer>();
    ecsManager->registerComponent<LineRenderer>();
    // ecsManager->registerComponent<SkyboxRenderer>();

    ecsManager->registerComponent<TextureEffectParam>();
}

void MyGame::RegisterUsingSystems() {
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
    ecsManager->registerSystem<SceneChangerSystem>();
    ecsManager->registerSystem<ButtonInputSystem>();
    ecsManager->registerSystem<ChangeSceneByButton>();
    ecsManager->registerSystem<LevelUIAdaptSystem>();
    ecsManager->registerSystem<LevelUIAnimationSystem>();
    ecsManager->registerSystem<LevelUIInitSystem>();
    ecsManager->registerSystem<LevelUIParentInitSystem>();
    ecsManager->registerSystem<TutorialMenuParentSystem>();
    ecsManager->registerSystem<TutorialMenuSystem>();
    ecsManager->registerSystem<MenuArrowSystem>();
    ecsManager->registerSystem<MenuSystem>();
    ecsManager->registerSystem<MenuFontSystem>();
    ecsManager->registerSystem<TutorialArrowSystem>();
    ecsManager->registerSystem<BlockColorChangeSystem>();
    ecsManager->registerSystem<SceneTransitionSystem>();
    ecsManager->registerSystem<MenutextSystem>();
    ecsManager->registerSystem<UpdateButtonColorByState>();
    ecsManager->registerSystem<UsingCameraSetSystem>();
    ecsManager->registerSystem<ResultScoreAdaptSystem>();
    ecsManager->registerSystem<ResultUIParentSystem>();
    ecsManager->registerSystem<ResultFontSystem>();
    ecsManager->registerSystem<ResultRankSystem>();
    ecsManager->registerSystem<ResultKeySystem>();
    ecsManager->registerSystem<GameEndUISystem>();
    ecsManager->registerSystem<TimerAnimationSystem>();
    ecsManager->registerSystem<TimeAdaptAinmationSystem>();
    ecsManager->registerSystem<DeleteBlockForAdvantageSystem>();
    ecsManager->registerSystem<PlusScoreAdaptUISystem>();
    ecsManager->registerSystem<AdapAnimationtBackTimerSystem>();
    ecsManager->registerSystem<PlusScoreIconSystem>();
    ecsManager->registerSystem<VignetteEffect>();
    ecsManager->registerSystem<TimerAdaptVignetSystem>();
    ecsManager->registerSystem<FloatingFloorAnimationSystem>();
    ecsManager->registerSystem<CameraShakeSystem>();
    ecsManager->registerSystem<BlockAdaptTextureSystem>();
    ecsManager->registerSystem<BlockApearSystem>();

    ecsManager->registerSystem<FloorUISystem>();
    ecsManager->registerSystem<CheckIsUnderPlayer>();

    /// ====================================================================================================
    // Initialize
    /// ====================================================================================================

    /// ===================================================================================================
    // Input
    /// ===================================================================================================

    /// ===================================================================================================
    // StateTransition
    /// ===================================================================================================

    /// =================================================================================================
    // Movement
    /// =================================================================================================
    ecsManager->registerSystem<MoveSystemByRigidBody>();

    /// =================================================================================================
    // Collision
    /// =================================================================================================
    ecsManager->registerSystem<CollisionCheckSystem>();

    /// =================================================================================================
    // Effect
    /// =================================================================================================
    ecsManager->registerSystem<EmitterWorkSystem>();
    ecsManager->registerSystem<PrimitiveNodeAnimationWorkSystem>();

    /// =================================================================================================
    // Render
    /// =================================================================================================
    ecsManager->registerSystem<ParticleRenderSystem>();
    ecsManager->registerSystem<SpriteRenderSystem>();
    ecsManager->registerSystem<TexturedMeshRenderSystem>();
#ifdef _DEBUG
    ecsManager->registerSystem<LineRenderSystem>(); // デバッグ用
#endif // _DEBUG
    ecsManager->registerSystem<ColliderRenderingSystem>();
    // ecsManager->registerSystem<SkyboxRender>();
    ecsManager->registerSystem<EffectTexturedMeshRenderSystem>();
    /// =================================================================================================
    // PostRender
    /// =================================================================================================
    ecsManager->registerSystem<GrayscaleEffect>();
    ecsManager->registerSystem<SmoothingEffect>();
}
