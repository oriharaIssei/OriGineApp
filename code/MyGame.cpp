#include "MyGame.h"

/// engine include
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
#include "module/editor/EditorGroup.h"
/// debugger
#include "ECSDebugger.h"
#include "logger/Logger.h"
#include "module/debugger/DebuggerGroup.h"
#endif // DEBUG

/// =====================================================
// Application Include
/// =====================================================
// component
#include "component/cameraController/CameraController.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"

// system
#include "system/Initialize/SettingGameCameraTarget.h"
#include "system/Input/CameraInputSystem.h"
#include "system/Input/PlayerInputSystem.h"
#include "system/Movement/FollowCameraUpdateSystem.h"
#include "system/Movement/PlayerMoveSystem.h"

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

        std::unique_ptr<GuiLogger> logger = std::make_unique<GuiLogger>();
        debuggerGroup->addDebugger(std::move(logger));

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

    ecsManager->registerComponent<Transform>();
    ecsManager->registerComponent<CameraTransform>();

    ecsManager->registerComponent<DirectionalLight>();
    ecsManager->registerComponent<PointLight>();
    ecsManager->registerComponent<SpotLight>();

    ecsManager->registerComponent<Rigidbody>();

    ecsManager->registerComponent<AABBCollider>();
    ecsManager->registerComponent<SphereCollider>();

    ecsManager->registerComponent<Emitter>();
    ecsManager->registerComponent<TextureEffectParam>();
    ecsManager->registerComponent<VignetteParam>();
    ecsManager->registerComponent<DistortionEffectParam>();

    ecsManager->registerComponent<Audio>();

    ecsManager->registerComponent<ModelNodeAnimation>();
    ecsManager->registerComponent<PrimitiveNodeAnimation>();

    ecsManager->registerComponent<ModelMeshRenderer>();
    ecsManager->registerComponent<PlaneRenderer>();
    ecsManager->registerComponent<SpriteRenderer>();
    ecsManager->registerComponent<LineRenderer>();
    ecsManager->registerComponent<SkyboxRenderer>();

    ecsManager->registerComponent<CameraController>();
    ecsManager->registerComponent<PlayerInput>();
    ecsManager->registerComponent<PlayerStatus>();
}

void MyGame::RegisterUsingSystems() {
    ECSManager* ecsManager = ECSManager::getInstance();

    /// ====================================================================================================
    // Initialize
    /// ====================================================================================================
    ecsManager->registerSystem<SettingGameCameraTarget>();

    /// ===================================================================================================
    // Input
    /// ===================================================================================================
    ecsManager->registerSystem<PlayerInputSystem>();
    ecsManager->registerSystem<CameraInputSystem>();

    /// ===================================================================================================
    // StateTransition
    /// ===================================================================================================

    /// =================================================================================================
    // Movement
    /// =================================================================================================
    ecsManager->registerSystem<MoveSystemByRigidBody>();
    ecsManager->registerSystem<FollowCameraUpdateSystem>();
    ecsManager->registerSystem<PlayerMoveSystem>();

    /// =================================================================================================
    // Collision
    /// =================================================================================================
    ecsManager->registerSystem<CollisionCheckSystem>();

    /// =================================================================================================
    // Effect
    /// =================================================================================================
    ecsManager->registerSystem<EmitterWorkSystem>();
    ecsManager->registerSystem<PrimitiveNodeAnimationWorkSystem>();
    ecsManager->registerSystem<TextureEffectAnimation>();

    /// =================================================================================================
    // Render
    /// =================================================================================================
    ecsManager->registerSystem<ParticleRenderSystem>();
    ecsManager->registerSystem<SpriteRenderSystem>();
    ecsManager->registerSystem<TexturedMeshRenderSystem>();
    ecsManager->registerSystem<EffectTexturedMeshRenderSystem>();
    ecsManager->registerSystem<LineRenderSystem>();
    ecsManager->registerSystem<ColliderRenderingSystem>();
    ecsManager->registerSystem<SkyboxRender>();

    /// =================================================================================================
    // PostRender
    /// =================================================================================================
    ecsManager->registerSystem<GrayscaleEffect>();
    ecsManager->registerSystem<SmoothingEffect>();
    ecsManager->registerSystem<VignetteEffect>();
    ecsManager->registerSystem<DistortionEffect>();
}
