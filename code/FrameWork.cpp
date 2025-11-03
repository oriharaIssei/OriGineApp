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
#include "component/Camera/CameraController.h"
#include "component/MouseCondition.h"
#include "component/player/PlayerEffectControlParam.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"
#include "component/SceneChanger.h"
#include "component/spline/SplinePoints.h"
#include "component/Stage/Stage.h"
#include "component/Stage/StageFloor.h"
#include "component/Stage/StageWall.h"
#include "component/TimerComponent.h"

// application system
#include "system/collision/PlayerOnCollision.h"
#include "system/collision/TutorialColliderOnCollision.h"
#include "system/effect/CreateMeshFromSpline.h"
#include "system/effect/EffectOnPlayerGearup.h"
#include "system/effect/EffectOnPlayerRun.h"
#include "system/effect/TimerForSprite.h"
#include "system/initialize/CreateSpriteFromTimer.h"
#include "system/initialize/CreateStage.h"
#include "system/initialize/GetClearTime.h"
#include "system/initialize/SettingGameCameraTarget.h"
#include "system/initialize/TakePlayerToStartPosition.h"
#include "system/initialize/TrailEffectInitialize.h"
#include "system/input/ButtonInputSystem.h"
#include "system/input/CameraInputSystem.h"
#include "system/input/PlayerInputSystem.h"
#include "system/movement/BillboardTransform.h"
#include "system/movement/FollowCameraUpdateSystem.h"
#include "system/movement/MenuUpdate.h"
#include "system/movement/PlayerMoveSystem.h"
#include "system/movement/PlayerPathSplineGenerator.h"
#include "system/movement/PlayerUpdateOnTitle.h"
#include "system/render/StageDebugRender.h"
#include "system/transition/ApplyMouseConditionSystem.h"
#include "system/transition/ButtonGroupSystem.h"
#include "system/transition/ChangeSceneByButton.h"
#include "system/transition/ExitApplicationByButton.h"
#include "system/transition/FallDetectionSystem.h"
#include "system/transition/SceneTransition.h"
#include "system/transition/SetClearTime.h"
#include "system/transition/ShowGameUIByInputDevice.h"
#include "system/transition/SubSceneActivateByButton.h"
#include "system/transition/SubSceneDeactivateByButton.h"
#include "system/transition/TimeLimitJudgeSystem.h"
#include "system/transition/TimerCountDown.h"
#include "system/transition/TransitionPlayerState.h"
#include "system/transition/UpdateButtonColorByState.h"

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

    componentRegistry->registerComponent<MouseCondition>();

    componentRegistry->registerComponent<EntityReferenceList>();
    componentRegistry->registerComponent<SubScene>();

    componentRegistry->registerComponent<CameraTransform>();
    componentRegistry->registerComponent<Transform>();

    componentRegistry->registerComponent<Stage>();
    componentRegistry->registerComponent<StageFloor>();
    componentRegistry->registerComponent<StageWall>();

    componentRegistry->registerComponent<SplinePoints>();

    componentRegistry->registerComponent<Material>();
    componentRegistry->registerComponent<DirectionalLight>();
    componentRegistry->registerComponent<PointLight>();
    componentRegistry->registerComponent<SpotLight>();

    componentRegistry->registerComponent<ModelNodeAnimation>();
    componentRegistry->registerComponent<PrimitiveNodeAnimation>();
    componentRegistry->registerComponent<SkinningAnimationComponent>();
    componentRegistry->registerComponent<SpriteAnimation>();
    componentRegistry->registerComponent<MaterialAnimation>();

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
    componentRegistry->registerComponent<MaterialEffectPipeLine>();
    componentRegistry->registerComponent<GradationTextureComponent>();

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
    componentRegistry->registerComponent<PlayerEffectControlParam>();

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
    systemRegistry->registerSystem<CameraInitialize>();
    systemRegistry->registerSystem<RegisterWindowResizeEvent>();
    systemRegistry->registerSystem<TrailEffectInitialize>();

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
    systemRegistry->registerSystem<ShowGameUIByInputDevice>();
    systemRegistry->registerSystem<ApplyMouseConditionSystem>();
    systemRegistry->registerSystem<SubSceneActivateByButton>();
    systemRegistry->registerSystem<SubSceneDeactivateByButton>();
    systemRegistry->registerSystem<ExitApplicationByButton>();

    /// =================================================================================================
    // Movement
    /// =================================================================================================
    systemRegistry->registerSystem<MoveSystemByRigidBody>();
    systemRegistry->registerSystem<SubSceneUpdate>();
    systemRegistry->registerSystem<BillboardTransform>();
    systemRegistry->registerSystem<FollowCameraUpdateSystem>();
    systemRegistry->registerSystem<PlayerMoveSystem>();

    systemRegistry->registerSystem<PlayerUpdateOnTitle>();

    systemRegistry->registerSystem<PlayerPathSplineGenerator>();

    systemRegistry->registerSystem<MenuUpdate>();

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
    systemRegistry->registerSystem<SkinningAnimationSystem>();
    systemRegistry->registerSystem<SpriteAnimationSystem>();
    systemRegistry->registerSystem<GpuParticleEmitterWorkSystem>();
    systemRegistry->registerSystem<MaterialAnimationWorkSystem>();
    systemRegistry->registerSystem<MaterialEffect>();
    systemRegistry->registerSystem<CreateMeshFromSpline>();

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
    systemRegistry->registerSystem<LineRenderSystem>();
    systemRegistry->registerSystem<GpuParticleRenderSystem>();

    systemRegistry->registerSystem<TimerForSprite>();

#ifndef _RELEASE
    systemRegistry->registerSystem<SkeletonRenderSystem>();
    systemRegistry->registerSystem<ColliderRenderingSystem>();
    systemRegistry->registerSystem<VelocityRenderingSystem>();
#endif // _RELEASE

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
    systemRegistry->registerSystem<GradationEffect>();
}
