#include "MyGame.h"

/// engine include
#include "winApp/WinApp.h"
#define ENGINE_INCLUDE
#define ENGINE_ECS
#include <EngineInclude.h>

// scene
#include "iScene/IScene.h"
#include "sceneManager/SceneManager.h"

/// lib
#include "globalVariables/GlobalVariables.h"

#ifdef _DEBUG
/// editor
#include "ECSEditor.h"
#include "module/editor/EditorController.h"
/// debugger
#include "ECSDebugger.h"
#include "logger/Logger.h"
#include "module/debugger/DebuggerGroup.h"
#endif // DEBUG

/// =====================================================
// Application Include
/// =====================================================
// component
#include "component/Button.h"
#include "component/cameraController/CameraController.h"
#include "component/effect/CameraAction.h"
#include "component/effect/post/RadialBlurParam.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/SceneChanger.h"
#include "component/TimerComponent.h"

// system
#include "system/Collision/PlayerOnCollision.h"
#include "system/effect/CameraActionSystem.h"
#include "system/effect/EffectOnPlayerGearup.h"
#include "system/effect/TimerForSprite.h"
#include "system/Initialize/CreateSpriteFromTimer.h"
#include "system/Initialize/SettingGameCameraTarget.h"
#include "system/Initialize/TakePlayerToStartPosition.h"
#include "system/Input/ButtonInputSystem.h"
#include "system/Input/CameraInputSystem.h"
#include "system/Input/PlayerInputSystem.h"
#include "system/Movement//BillboardTransform.h"
#include "system/Movement/FollowCameraUpdateSystem.h"
#include "system/Movement/PlayerMoveSystem.h"
#include "system/postRender/RadialBlurEffect.h"
#include "system/Transition/ChangeSceneByButton.h"
#include "system/Transition/FallDetectionSystem.h"
#include "system/Transition/TimerCountDown.h"
#include "system/Transition/TransitionPlayerState.h"
#include "system/Transition/TransitionSceneByTimer.h"
#include "system/Transition/UpdateButtonColorByState.h"

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
        EditorController* editorController   = EditorController::getInstance();
        std::unique_ptr<ECSEditor> ecsEditor = std::make_unique<ECSEditor>();
        editorController->addEditor(std::move(ecsEditor));
        std::unique_ptr<GlobalVariablesEditor> globalVariables = std::make_unique<GlobalVariablesEditor>();
        editorController->addEditor(std::move(globalVariables));

        // controller Initialize
        editorController->Initialize();
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

        std::unique_ptr<GuiLogger> logger = std::make_unique<GuiLogger>();
        debuggerGroup->addDebugger(std::move(logger));

        // gorup Initialize
        debuggerGroup->Initialize();
    }

#endif // DEBUG
}

void MyGame::Finalize() {
#ifdef _DEBUG
    EditorController* editorController = EditorController::getInstance();
    DebuggerGroup* debuggerGroup       = DebuggerGroup::getInstance();
    editorController->Finalize();
    debuggerGroup->Finalize();
#endif // _DEBUG

    SceneFinalize();

    sceneManager_->Finalize();
    engine_->Finalize();
}

void MyGame::Run() {
    while (true) {
        if (engine_->ProcessMessage() || sceneManager_->isExitGame()) {
            break;
        }

        engine_->BeginFrame();

#ifdef _DEBUG
        sceneManager_->DebugUpdate();
#else
        if (!engine_->getWinApp()->isActive()) {
            continue;
        }
#endif // DEBUG

        sceneManager_->Update();

        engine_->EndFrame();
    }
}

void MyGame::RegisterUsingComponents() {
    ECSManager* ecsManager = ECSManager::getInstance();

    ecsManager->registerComponent<Audio>();

    ecsManager->registerComponent<Button>();

    ecsManager->registerComponent<Transform>();
    ecsManager->registerComponent<CameraTransform>();

    ecsManager->registerComponent<DirectionalLight>();
    ecsManager->registerComponent<PointLight>();
    ecsManager->registerComponent<SpotLight>();

    ecsManager->registerComponent<Rigidbody>();

    ecsManager->registerComponent<AABBCollider>();
    ecsManager->registerComponent<SphereCollider>();
    ecsManager->registerComponent<CollisionPushBackInfo>();

    ecsManager->registerComponent<Emitter>();
    ecsManager->registerComponent<TextureEffectParam>();

    ecsManager->registerComponent<VignetteParam>();
    ecsManager->registerComponent<DistortionEffectParam>();
    ecsManager->registerComponent<DistortionEffectParam>();
    ecsManager->registerComponent<RadialBlurParam>();

    ecsManager->registerComponent<ModelNodeAnimation>();
    ecsManager->registerComponent<PrimitiveNodeAnimation>();
    ecsManager->registerComponent<CameraAction>();

    ecsManager->registerComponent<ModelMeshRenderer>();
    ecsManager->registerComponent<PlaneRenderer>();
    ecsManager->registerComponent<RingRenderer>();
    ecsManager->registerComponent<SpriteRenderer>();
    ecsManager->registerComponent<LineRenderer>();
    ecsManager->registerComponent<SkyboxRenderer>();

    ecsManager->registerComponent<CameraController>();
    ecsManager->registerComponent<PlayerInput>();
    ecsManager->registerComponent<PlayerStatus>();

    ecsManager->registerComponent<SceneChanger>();
    ecsManager->registerComponent<TimerComponent>();
    ecsManager->registerComponent<TimerForSpriteComponent>();
}

void MyGame::RegisterUsingSystems() {
    ECSManager* ecsManager = ECSManager::getInstance();

    /// ====================================================================================================
    // Initialize
    /// ====================================================================================================
    ecsManager->registerSystem<SettingGameCameraTarget>();
    ecsManager->registerSystem<CreateSpriteFromTimer>();
    ecsManager->registerSystem<TakePlayerToStartPosition>();

    /// ===================================================================================================
    // Input
    /// ===================================================================================================
    ecsManager->registerSystem<PlayerInputSystem>();
    ecsManager->registerSystem<CameraInputSystem>();
    ecsManager->registerSystem<ButtonInputSystem>();

    /// ===================================================================================================
    // StateTransition
    /// ===================================================================================================
    ecsManager->registerSystem<TransitionPlayerState>();
    ecsManager->registerSystem<TimerCountDown>();
    ecsManager->registerSystem<TransitionSceneByTimer>();
    ecsManager->registerSystem<FallDetectionSystem>();
    ecsManager->registerSystem<UpdateButtonColorByState>();
    ecsManager->registerSystem<ChangeSceneByButton>();

    /// =================================================================================================
    // Movement
    /// =================================================================================================
    ecsManager->registerSystem<MoveSystemByRigidBody>();
    ecsManager->registerSystem<FollowCameraUpdateSystem>();
    ecsManager->registerSystem<PlayerMoveSystem>();
    ecsManager->registerSystem<BillboardTransform>();

    /// =================================================================================================
    // Collision
    /// =================================================================================================
    ecsManager->registerSystem<CollisionCheckSystem>();
    ecsManager->registerSystem<CollisionPushBackSystem>();
    ecsManager->registerSystem<PlayerOnCollision>();

    /// =================================================================================================
    // Effect
    /// =================================================================================================
    ecsManager->registerSystem<EmitterWorkSystem>();
    ecsManager->registerSystem<PrimitiveNodeAnimationWorkSystem>();
    ecsManager->registerSystem<TextureEffectAnimation>();
    ecsManager->registerSystem<EffectOnPlayerGearup>();
    ecsManager->registerSystem<TimerForSprite>();
    ecsManager->registerSystem<CameraActionSystem>();

    /// =================================================================================================
    // Render
    /// =================================================================================================
    ecsManager->registerSystem<ParticleRenderSystem>();
    ecsManager->registerSystem<SpriteRenderSystem>();
    ecsManager->registerSystem<TexturedMeshRenderSystem>();
    ecsManager->registerSystem<EffectTexturedMeshRenderSystem>();
    ecsManager->registerSystem<LineRenderSystem>();

#ifdef _DEBUG
    ecsManager->registerSystem<ColliderRenderingSystem>();
#endif // _DEBUG

    ecsManager->registerSystem<SkyboxRender>();

    /// =================================================================================================
    // PostRender
    /// =================================================================================================
    ecsManager->registerSystem<GrayscaleEffect>();
    ecsManager->registerSystem<SmoothingEffect>();
    ecsManager->registerSystem<VignetteEffect>();
    ecsManager->registerSystem<RadialBlurEffect>();
    ecsManager->registerSystem<DistortionEffect>();
}
