#include "FrameWork.h"

/// engine include
#define ENGINE_INCLUDE
#define ENGINE_ECS
#define ENGINE_COMPONENTS
#define ENGINE_SYSTEMS
#include <EngineInclude.h>

#include "globalVariables/GlobalVariables.h"

// application component
#include "component/Button.h"
#include "component/ButtonGroup.h"
#include "component/cameraController/CameraController.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"
#include "component/SceneChanger.h"
#include "component/Stage/Stage.h"
#include "component/Stage/StageFloor.h"
#include "component/Stage/StageWall.h"
#include "component/TimerComponent.h"

// application system
#include "system/collision/PlayerOnCollision.h"
#include "system/collision/TutorialColliderOnCollision.h"
#include "system/effect/EffectOnPlayerGearup.h"
#include "system/effect/EffectOnPlayerRun.h"
#include "system/effect/TimerForSprite.h"
#include "system/Initialize/CreateSpriteFromTimer.h"
#include "system/Initialize/CreateStage.h"
#include "system/Initialize/GetClearTime.h"
#include "system/Initialize/SettingGameCameraTarget.h"
#include "system/Initialize/TakePlayerToStartPosition.h"
#include "system/Input/ButtonInputSystem.h"
#include "system/Input/CameraInputSystem.h"
#include "system/Input/PlayerInputSystem.h"
#include "system/Movement/BillboardTransform.h"
#include "system/Movement/FollowCameraUpdateSystem.h"
#include "system/Movement/PlayerMoveSystem.h"
#include "system/Movement/UpdateCameraForward.h"
#include "system/render/StageDebugRender.h"
#include "system/Transition/ButtonGroupSystem.h"
#include "system/Transition/ChangeSceneByButton.h"
#include "system/Transition/FallDetectionSystem.h"
#include "system/Transition/SceneTransition.h"
#include "system/Transition/SetClearTime.h"
#include "system/Transition/TimeLimitJudgeSystem.h"
#include "system/Transition/TimerCountDown.h"
#include "system/Transition/TransitionPlayerState.h"
#include "system/Transition/UpdateButtonColorByState.h"

#ifndef _RELEASE
#include "system/Transition/StageReloadSystem.h"
#endif // _RELEASE

//
// / =====================================================
// Application Include
/// =====================================================
// component

FrameWork::FrameWork() {}

FrameWork::~FrameWork() {}

//! TODO : 場所 変更 FrameWork はUserに分かりづらすぎる

void RegisterUsingComponents() {
    ComponentRegistry* componentRegistry = ComponentRegistry::getInstance();

    componentRegistry->registerComponent<Audio>();

    componentRegistry->registerComponent<EntityReferenceList>();
    componentRegistry->registerComponent<SubScene>();

    componentRegistry->registerComponent<CameraTransform>();
    componentRegistry->registerComponent<Transform>();

    componentRegistry->registerComponent<Stage>();
    componentRegistry->registerComponent<StageFloor>();
    componentRegistry->registerComponent<StageWall>();

    componentRegistry->registerComponent<DirectionalLight>();
    componentRegistry->registerComponent<PointLight>();
    componentRegistry->registerComponent<SpotLight>();

    componentRegistry->registerComponent<ModelNodeAnimation>();
    componentRegistry->registerComponent<PrimitiveNodeAnimation>();
    componentRegistry->registerComponent<SkinningAnimationComponent>();
    componentRegistry->registerComponent<SpriteAnimation>();

    componentRegistry->registerComponent<AABBCollider>();
    componentRegistry->registerComponent<OBBCollider>();
    componentRegistry->registerComponent<SphereCollider>();
    componentRegistry->registerComponent<CollisionPushBackInfo>();
    componentRegistry->registerComponent<Rigidbody>();

    componentRegistry->registerComponent<Emitter>();
    componentRegistry->registerComponent<GpuParticleEmitter>();
    componentRegistry->registerComponent<DissolveEffectParam>();
    componentRegistry->registerComponent<DistortionEffectParam>();
    componentRegistry->registerComponent<RadialBlurParam>();
    componentRegistry->registerComponent<RandomEffectParam>();
    componentRegistry->registerComponent<VignetteParam>();
    componentRegistry->registerComponent<SpeedlineEffectParam>();
    componentRegistry->registerComponent<TextureEffectParam>();

    componentRegistry->registerComponent<ModelMeshRenderer>();
    componentRegistry->registerComponent<LineRenderer>();
    componentRegistry->registerComponent<PlaneRenderer>();
    componentRegistry->registerComponent<RingRenderer>();
    componentRegistry->registerComponent<BoxRenderer>();
    componentRegistry->registerComponent<SphereRenderer>();
    componentRegistry->registerComponent<SkyboxRenderer>();
    componentRegistry->registerComponent<SpriteRenderer>();

    componentRegistry->registerComponent<TimerComponent>();
    componentRegistry->registerComponent<TimerForSpriteComponent>();

    componentRegistry->registerComponent<CameraController>();

    componentRegistry->registerComponent<PlayerInput>();
    componentRegistry->registerComponent<PlayerStatus>();
    componentRegistry->registerComponent<PlayerState>();

    componentRegistry->registerComponent<Button>();
    componentRegistry->registerComponent<ButtonGroup>();
    componentRegistry->registerComponent<SceneChanger>();
}

void RegisterUsingSystems() {
    SystemRegistry* systemRegistry = SystemRegistry::getInstance();

    /// ====================================================================================================
    // Initialize
    /// ====================================================================================================
    systemRegistry->registerSystem<SettingGameCameraTarget>();
    systemRegistry->registerSystem<TakePlayerToStartPosition>();
    systemRegistry->registerSystem<GpuParticleInitialize>();
    systemRegistry->registerSystem<CreateStage>();
    systemRegistry->registerSystem<ResolveEntityReferences>();
    systemRegistry->registerSystem<CreateSpriteFromTimer>();
    systemRegistry->registerSystem<GetClearTime>();

    /// ===================================================================================================
    // Input
    /// ===================================================================================================
    systemRegistry->registerSystem<ButtonInputSystem>();
    systemRegistry->registerSystem<CameraInputSystem>();
    systemRegistry->registerSystem<PlayerInputSystem>();

    /// ===================================================================================================
    // StateTransition
    /// ===================================================================================================
    systemRegistry->registerSystem<ChangeSceneByButton>();
    systemRegistry->registerSystem<FallDetectionSystem>();
    systemRegistry->registerSystem<SceneTransition>();
    systemRegistry->registerSystem<TransitionPlayerState>();
    systemRegistry->registerSystem<UpdateButtonColorByState>();
    systemRegistry->registerSystem<ButtonGroupSystem>();
    systemRegistry->registerSystem<TimerCountDown>();
    systemRegistry->registerSystem<TimeLimitJudgeSystem>();
    systemRegistry->registerSystem<SetClearTime>();

#ifndef _RELEASE
    systemRegistry->registerSystem<StageReloadSystem>();
#endif // _RELEASE

    /// =================================================================================================
    // Movement
    /// =================================================================================================
    systemRegistry->registerSystem<MoveSystemByRigidBody>();
    systemRegistry->registerSystem<SubSceneUpdate>();

    systemRegistry->registerSystem<BillboardTransform>();
    systemRegistry->registerSystem<FollowCameraUpdateSystem>();
    systemRegistry->registerSystem<PlayerMoveSystem>();
    systemRegistry->registerSystem<UpdateCameraForward>();

    /// =================================================================================================
    // Collision
    /// =================================================================================================
    systemRegistry->registerSystem<CollisionCheckSystem>();
    systemRegistry->registerSystem<CollisionPushBackSystem>();

    systemRegistry->registerSystem<PlayerOnCollision>();
    systemRegistry->registerSystem<TutorialColliderOnCollision>();

    /// =================================================================================================
    // Effect
    /// =================================================================================================
    systemRegistry->registerSystem<EmitterWorkSystem>();
    systemRegistry->registerSystem<PrimitiveNodeAnimationWorkSystem>();
    systemRegistry->registerSystem<TextureEffectAnimation>();
    systemRegistry->registerSystem<SkinningAnimationSystem>();
    systemRegistry->registerSystem<SpriteAnimationSystem>();
    systemRegistry->registerSystem<GpuParticleEmitterWorkSystem>();

    systemRegistry->registerSystem<EffectOnPlayerGearup>();
    systemRegistry->registerSystem<EffectOnPlayerRun>();

    /// =================================================================================================
    // Render
    /// =================================================================================================
    systemRegistry->registerSystem<SkyboxRender>();
    systemRegistry->registerSystem<SpriteRenderSystem>();
    systemRegistry->registerSystem<BackGroundSpriteRenderSystem>();
    systemRegistry->registerSystem<ParticleRenderSystem>();
    systemRegistry->registerSystem<TexturedMeshRenderSystem>();
    systemRegistry->registerSystem<SkinningMeshRenderSystem>();
    systemRegistry->registerSystem<EffectTexturedMeshRenderSystem>();
    systemRegistry->registerSystem<LineRenderSystem>();
    systemRegistry->registerSystem<GpuParticleRenderSystem>();

    systemRegistry->registerSystem<TimerForSprite>();

    systemRegistry->registerSystem<SkeletonRenderSystem>();
    systemRegistry->registerSystem<ColliderRenderingSystem>();
    systemRegistry->registerSystem<VelocityRenderingSystem>();

    systemRegistry->registerSystem<StageDebugRender>();

    /// =================================================================================================
    // PostRender
    /// =================================================================================================
    systemRegistry->registerSystem<GrayscaleEffect>();
    systemRegistry->registerSystem<SmoothingEffect>();
    systemRegistry->registerSystem<VignetteEffect>();
    systemRegistry->registerSystem<DistortionEffect>();
    systemRegistry->registerSystem<DissolveEffect>();
    systemRegistry->registerSystem<RandomEffect>();
    systemRegistry->registerSystem<RadialBlurEffect>();
    systemRegistry->registerSystem<SubSceneRender>();
    systemRegistry->registerSystem<SpeedlineEffect>();
}
