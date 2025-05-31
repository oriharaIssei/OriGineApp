#include "MyGame.h"

/// engine

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

#include "model/ModelManager.h"
#include "texture/TextureManager.h"

#include "component/BigBom/BigBomSpawner.h"
#include "component/BigBom/BigBomStatus.h"
#include "component/BigBom/BigExplotionCollision.h"
#include "component/Block/BlockCombinationStatus.h"
#include "component/Block/BlockFrontPlaneStatus.h"
#include "component/Block/BlockManager.h"
#include "component/Block/BlockStatus.h"
#include "component/Bom/BomSpawner.h"
#include "component/Bom/BomStatus.h"
#include "component/Bom/ExplotionCollision.h"
#include "component/Button.h"
#include "component/Combo/ComboStatus.h"
#include "component/Combo/ComboUIStatus.h"
#include "component/effect/post/VignetteParam.h"
#include "component/effect/TextureEffectParam.h"
#include "component/EffectByBlock/EffectByBlockSpawner.h"
#include "component/EffectByBlock/EffectByBlockUIStatus.h"
#include "component/Field/FieldStatus.h"
#include "component/FloorUI/FloorUIController.h"
#include "component/FloorUI/FloorUIStatus.h"
#include "component/GameCamera/CameraRenditionStatus.h"
#include "component/GameEndUI/GameEndUIStatus.h"
#include "component/GameStart/GameStartStatus.h"
#include "component/LevelUPUI/LevelUIParentStatus.h"
#include "component/LevelUPUI/LevelUIStatus.h"
#include "component/Menu/MenuStatus.h"
#include "component/Menu/TutorialArrowStatus.h"
#include "component/Menu/TutorialMenuParentStatus.h"
#include "component/Menu/TutorialMenuStatus.h"
#include "component/OperateUI/OperateUIStatus.h"
#include "component/Piller/FloatingFloorAnimationStatus.h"
#include "component/Piller/FloatingFloorSpawner.h"
#include "component/Piller/FloatingFloorStatus.h"
#include "component/Player/PlayerAnimationStatus.h"
#include "component/Player/PlayerStates.h"
#include "component/ResultUI/ResultFontStatus.h"
#include "component/ResultUI/ResultUIkeyStatus.h"
#include "component/ResultUI/ResultUIParentStatus.h"
#include "component/ResultUI/ResultUIRankStatus.h"
#include "component/ResultUI/ResultUIScoreStatus.h"
#include "component/SceneChanger.h"
#include "component/SceneChanger/SceneChangerStatus.h"
#include "component/SceneTransition/SceneTransition.h"
#include "component/Score/ScoreStatus.h"
#include "component/Score/ScoreUIStatus.h"
#include "component/Scrap/ScrapSpawner.h"
#include "component/Scrap/ScrapStatus.h"
#include "component/Timer/TimerAnimationStatus.h"
#include "component/Timer/TimerStatus.h"
#include "component/Timer/TimerUIStatus.h"
#include"component/animation/SpriteAnimation.h"
#include"component/DeleteEntityStatus/DeleteEntityStatus.h"
#include"component/ScaleByBlockStatus/ScaleByBlockStatus.h"

// system
#include "system/BigBom/BigBomCollisionExSystem.h"
#include "system/BigBom/BigBomInputSystem.h"
#include "system/BigBom/BigBomLaunchSystem.h"
#include "system/BigBom/DeleteBigBomSystem.h"
#include "system/BigBom/DeleteBigExplotionCollision.h"
#include "system/Block/BackPlaneChangeCloseSystem.h"
#include "system/Block/BackPlaneCloseSystem.h"
#include "system/Block/BlockApearSystem.h"
#include "system/Block/BlockColorChangeSystem.h"
#include "system/Block/BlockExBomCollision.h"
#include "system/Block/BlockFloorCollision.h"
#include "system/Block/BlockMoveSystem.h"
#include "system/Block/BlockSpawnSystem.h"
#include "system/Block/BreakBlockSystem.h"
#include "system/Block/DeleteBlockForAdvantageSystem.h"
#include "system/Block/DeleteBlockSystem.h"
#include "system/Block/FrontPlaneDeleteSystem.h"
#include "system/Block/MoveTenpoSystem.h"
#include "system/Bom/BomCollisionExSystem.h"
#include "system/Bom/BomExplotionSystem.h"
#include "system/Bom/DeleteBomSystem.h"
#include "system/Bom/DeleteExplotionCollision.h"
#include "system/Bom/PutBomSystem.h"
#include "system/ButtonInputSystem.h"
#include "system/ChangeSceneByButton.h"
#include "system/Combo/ComboSystem.h"
#include "system/Combo/ComboUIScrollSystem.h"
#include "system/EffectByBlock/EffectByBlockDeleteSystem.h"
#include "system/EffectByBlock/EffectByBlockDrawSystem.h"
#include "system/FloatingFloor/CanageStateFallSystem.h"
#include "system/FloatingFloor/CheckIsUnderPlayer.h"
#include "system/FloatingFloor/CreateFloorSystem.h"
#include "system/FloatingFloor/DeleteFloatingFloorSystem.h"
#include "system/FloatingFloor/FloatingFloorAnimationSystem.h"
#include "system/FloatingFloor/FloatingFloorDamageSystem.h"
#include "system/FloatingFloor/FloatingFloorFallSystem.h"
#include "system/FloatingFloor/FloatingFloorRevivalSystem.h"
#include "system/Floor/DeleteFloorSystem.h"
#include "system/Floor/FloorUpdateMatrixSystem.h"
#include "system/FloorUI/FloorUISystem.h"
#include "system/GameCamera/CameraShakeSystem.h"
#include "system/GameEndUI/GameEndUISystem.h"
#include "system/GameStart/AdaptGoSystem.h"
#include "system/GameStart/AdaptPurposeSystem.h"
#include "system/GameStart/AdaptReadySystem.h"
#include "system/GameStart/GameStartBackGroundSystem.h"
#include "system/GameStart/GameStartRenditionSystem.h"
#include "system/LeverlUP/LevelUIAdaptSystem.h"
#include "system/LeverlUP/LevelUIAnimationSystem.h"
#include "system/LeverlUP/LevelUIInitSystem.h"
#include "system/LeverlUP/LevelUIParentInitSystem.h"
#include "system/Matrix/UpdateMatrixSystem.h"
#include "system/Menu/MenuArrowSystem.h"
#include "system/Menu/MenuFontSystem.h"
#include "system/Menu/MenuSystem.h"
#include "system/Menu/MenutextSystem.h"
#include "system/Menu/TutorialArrowSystem.h"
#include "system/Menu/TutorialMenuParentSystem.h"
#include "system/Menu/TutorialMenuSystem.h"
#include "system/OperateUI/OperateUISystem.h"
#include "system/Player/PlayerAnimationSystem.h"
#include "system/Player/PlayerCreateBigBomSystem.h"
#include "system/Player/PlayerFollowCameraSystem.h"
#include "system/Player/PlayerInput.h"
#include "system/Player/PlayerMoveSystem.h"
#include "system/postRender/VignetteEffect.h"
#include "system/ResultUI/ResultFontSystem.h"
#include "system/ResultUI/ResultKeySystem.h"
#include "system/ResultUI/ResultRankSystem.h"
#include "system/ResultUI/ResultScoreAdaptSystem.h"
#include "system/ResultUI/ResultUIParentSystem.h"
#include "system/SceneChanger/SceneChangerSystem.h"
#include "system/SceneTransitionSystem/SceneTransitionSystem.h"
#include "system/Score/PlusScoreAdaptUISystem.h"
#include "system/Score/PlusScoreIconSystem.h"
#include "system/Score/ScoreIncrementSystem.h"
#include "system/Score/ScoreUIScrollSystem.h"
#include "system/scrap/ScrapDeleteSystem.h"
#include "system/scrap/ScrapFallSystem.h"
#include "system/scrap/ScrapToPlayerCollisionSystem.h"
#include "system/Timer/AdapAnimationtBackTimerSystem.h"
#include "system/Timer/TimeAdaptAinmationSystem.h"
#include "system/Timer/TimerAdaptVignetSystem.h"
#include "system/Timer/TimerAnimationSystem.h"
#include "system/Timer/TimerDecrementSystem.h"
#include "system/Timer/TimerUIScrollSystem.h"
#include "system/UpdateButtonColorByState.h"
#include "system/UsingCameraSetSystem.h"
#include"system/Block/SkullSinisterEffectSystem.h"
#include"system/DeleteEntitySystem/DeleteEntitySystem.h"
#include"system/ScaleByBlock/ScaleByBlockSystem.h"

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
    ResourceAllLoad();

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
    ecsManager->registerComponent<PlayerAnimationStatus>();
    ecsManager->registerComponent<GameStartStatus>();
    ecsManager->registerComponent<BlockFrontPlaneStatus>();
    ecsManager->registerComponent<DeleteEntityStatus>();
    ecsManager->registerComponent<ScaleByBlockStatus>();

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
    ecsManager->registerComponent<SpriteAnimation>();

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
    ecsManager->registerSystem<BackPlaneChangeCloseSystem>();
    ecsManager->registerSystem<BlockApearSystem>();
    ecsManager->registerSystem<PlayerAnimationSystem>();
    ecsManager->registerSystem<AdaptGoSystem>();
    ecsManager->registerSystem<AdaptPurposeSystem>();
    ecsManager->registerSystem<AdaptReadySystem>();
    ecsManager->registerSystem<GameStartBackGroundSystem>();
    ecsManager->registerSystem<GameStartRenditionSystem>();
    ecsManager->registerSystem<SkullSinisterEffectSystem>();
    ecsManager->registerSystem<DeleteEntitySystem>();
    ecsManager->registerSystem<ScaleByBlockSystem>();

    ecsManager->registerSystem<BackPlaneCloseSystem>();
    ecsManager->registerSystem<FrontPlaneDeleteSystem>();

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
    ecsManager->registerSystem<SpriteAnimationSystem>();

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

void MyGame::ResourceAllLoad() {
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/anchorPoint.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/DescriptionArrowLeft.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/DescriptionArrowRight.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/gameEnd.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/gameStartText.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/GameToTitleText.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/image.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/kira.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/Menu_Fream.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/MenuArrow.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/NoBreak.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/ready.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/ResultToTitle.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/Retry.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/ReturnGameText.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/SeeDescription.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/title.png");

    for (int32_t i = 0; i < 10; ++i) {
        TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/UI/BlueNumbers/bluenumber" + std::to_string(i).c_str() + ".png");
    }

    for (int32_t i = 0; i < 10; ++i) {
        TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/UI/GreenNumbers/greennumber" + std::to_string(i).c_str() + ".png");
    }

    for (int32_t i = 0; i < 10; ++i) {
        TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/UI/Numbers/number" + std::to_string(i).c_str() + ".png");
    }

    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/UI/Time/TimeNum.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/UI/Time/TimeSymbol.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/UI/AddTimeUI.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/UI/AddTimeUI-sheet.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/UI/GameToTitleUI.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/UI/menuOperate.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/UI/menuUP.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/UI/Numbers_Blue.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/UI/Numbers_Green.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/UI/PlaySpaceUI.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/UI/PlaySpaceUI_Explosion.png");

    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/Explanation/Explanation_01.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/Explanation/Explanation_02.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/Explanation/Explanation_03.png");
    TextureManager::LoadTexture(kApplicationResourceDirectory + "/Texture/Explanation/Explanation_04.png");

    /// model
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/AdvantageBlock", "AdvantageBlock.gltf");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/BLock", "BLock.gltf");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/SkullBlock", "SkullBlock.gltf");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/Plane", "Plane.gltf");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/Bomb", "Bomb.obj");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/Field", "Field.obj");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/Floor", "Floor.obj");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/Floor_Gold", "Floor_Gold.obj");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/Floor_Silver", "FloorSilver.obj");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/Floor_Break", "Floor_Break.obj");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/Floor_Break_Gold", "Floor_Break_Gold.obj");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/Floor_Break_Silver", "Floor_Break_Silver.obj");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/Player", "Player.gltf");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/Scrap", "Scrap.obj");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/hook", "hook.obj");
    /*ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/Floor_Gold_Text", "Floor_Gold_Text.gltf");
    ModelManager::getInstance()->Create(kApplicationResourceDirectory + "/Models/Floor_Silver_Text", "Floor_Silver_Text.gltf");*/
}
