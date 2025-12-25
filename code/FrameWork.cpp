#include "FrameWork.h"

/// engine include
#define ENGINE_INCLUDE
#define ENGINE_ECS
#define ENGINE_COMPONENTS
#define ENGINE_SYSTEMS
#include <EngineInclude.h>

// application component
#include "component/Camera/CameraController.h"
#include "component/camera/CameraMotionBob.h"
#include "component/Camera/CameraShakeSourceComponent.h"
#include "component/ghost/GhostReplayComponent.h"
#include "component/ghost/PlayRecordeComponent.h"
#include "component/LookAtFromTransforms.h"
#include "component/MouseCondition.h"
#include "component/player/PlayerEffectControlParam.h"
#include "component/player/PlayerInput.h"
#include "component/player/PlayerStatus.h"
#include "component/player/state/PlayerState.h"
#include "component/PlayerStateOverrideCondition.h"
#include "component/SceneChanger.h"
#include "component/spline/SplinePoints.h"
#include "component/spline/TireSplinePoints.h"
#include "component/stage/StageData.h"
#include "component/TimerComponent.h"
#include "component/ui/Button.h"
#include "component/ui/ButtonGroup.h"
#include "component/ui/SpeedFor3dUIComponent.h"

// application system
#include "system/collision/PlayerOnCollision.h"
#include "system/collision/ShadowCastSystem.h"
#include "system/collision/TutorialColliderOnCollision.h"
#include "system/effect/CameraShake.h"
#include "system/effect/CreateMeshFromSpline.h"
#include "system/effect/CreateMeshFromTireSpline.h"
#include "system/effect/EffectOnPlayerGearup.h"
#include "system/effect/EffectOnPlayerRun.h"
#include "system/effect/PenaltyTimeSpriteUpdate.h"
#include "system/effect/PlayerSpeedFor3dUI.h"
#include "system/effect/TimerForSprite.h"
#include "system/initialize/BackFireInitialize.h"
#include "system/Initialize/CreatePlaneFromSpeed.h"
#include "system/initialize/CreateSpriteFromTimer.h"
#include "system/initialize/GetClearTime.h"
#include "system/Initialize/GhostInitializeSystem.h"
#include "system/Initialize/InitializeMouseCondition.h"
#include "system/Initialize/PlayRecorderInitialize.h"
#include "system/Initialize/SelectPreviewSceneInitialize.h"
#include "system/initialize/SettingGameCameraTarget.h"
#include "system/initialize/StartTimerInitialize.h"
#include "system/initialize/TakePlayerToStartPosition.h"
#include "system/initialize/TakeToGoalPosition.h"
#include "system/Initialize/Ui3dObjectInitialize.h"
#include "system/input/ButtonInputSystem.h"
#include "system/input/CameraInputSystem.h"
#include "system/Input/GhostInputUpdate.h"
#include "system/input/PlayerInputSystem.h"
#include "system/Input/PlayRecordSystem.h"
#include "system/movement/BillboardTransform.h"
#include "system/movement/FollowCameraUpdateSystem.h"
#include "system/Movement/LookAtFromTransformsSystem.h"
#include "system/movement/PauseMainSceneSystem.h"
#include "system/movement/PlayerMoveSystem.h"
#include "system/movement/PlayerPathSplineGenerator.h"
#include "system/movement/PlayerUpdateOnTitle.h"
#include "system/movement/RestartSystem.h"
#include "system/movement/SceneTransitionRequestReceiverSystem.h"
#include "system/movement/SceneTransitionRequestSenderSystem.h"
#include "system/Movement/ShadowPlaneSyncPlayerPosition.h"
#include "system/movement/StartSequenceSystem.h"
#include "system/Movement/TireTrailGenerateSystem.h"
#include "system/Movement/Ui3dUpdateSystem.h"
#include "system/transition/ApplyMouseConditionSystem.h"
#include "system/transition/ButtonGroupSystem.h"
#include "system/transition/ButtonScenePreviewSystem.h"
#include "system/Transition/CameraMotionBobSystem.h"
#include "system/transition/ChangeSceneByButton.h"
#include "system/transition/ExitApplicationByButton.h"
#include "system/transition/FallDetectionSystem.h"
#include "system/transition/GameFailedSceneLaunchSystem.h"
#include "system/transition/PenaltySystem.h"
#include "system/Transition/PlayerStateOverrideSystem.h"
#include "system/transition/RestartOnButtonPressSystem.h"
#include "system/transition/SceneTransition.h"
#include "system/transition/ShowGameUIByInputDevice.h"
#include "system/transition/SubSceneActivateByButton.h"
#include "system/transition/SubSceneDeactivateByButton.h"
#include "system/transition/TimeLimitJudgeSystem.h"
#include "system/transition/TimerCountDown.h"
#include "system/transition/TransitionPlayerState.h"
#include "system/transition/UpdateButtonColorByState.h"

using namespace OriGine;

/// =====================================================
// Application Include
/// =====================================================
// component

FrameWork::FrameWork() {}
FrameWork::~FrameWork() {}

//! TODO : 場所 変更 FrameWork はUserに分かりづらすぎる

void RegisterUsingComponents() {
    ComponentRegistry* componentRegistry = ComponentRegistry::GetInstance();

    componentRegistry->RegisterComponent<Audio>();

    componentRegistry->RegisterComponent<MouseCondition>();

    componentRegistry->RegisterComponent<EntityReferenceList>();
    componentRegistry->RegisterComponent<SubScene>();

    componentRegistry->RegisterComponent<CameraTransform>();
    componentRegistry->RegisterComponent<Transform>();

    componentRegistry->RegisterComponent<LookAtFromTransforms>();

    componentRegistry->RegisterComponent<StageData>();

    componentRegistry->RegisterComponent<SplinePoints>();
    componentRegistry->RegisterComponent<TireSplinePoints>();

    componentRegistry->RegisterComponent<GhostReplayComponent>();
    componentRegistry->RegisterComponent<PlayRecordeComponent>();

    componentRegistry->RegisterComponent<Material>();
    componentRegistry->RegisterComponent<DirectionalLight>();
    componentRegistry->RegisterComponent<PointLight>();
    componentRegistry->RegisterComponent<SpotLight>();

    componentRegistry->RegisterComponent<CameraAction>();
    componentRegistry->RegisterComponent<ModelNodeAnimation>();
    componentRegistry->RegisterComponent<PrimitiveNodeAnimation>();
    componentRegistry->RegisterComponent<TransformAnimation>();
    componentRegistry->RegisterComponent<SkinningAnimationComponent>();
    componentRegistry->RegisterComponent<SpriteAnimation>();
    componentRegistry->RegisterComponent<MaterialAnimation>();

    componentRegistry->RegisterComponent<AABBCollider>();
    componentRegistry->RegisterComponent<OBBCollider>();
    componentRegistry->RegisterComponent<SphereCollider>();
    componentRegistry->RegisterComponent<CollisionPushBackInfo>();
    componentRegistry->RegisterComponent<Rigidbody>();

    componentRegistry->RegisterComponent<Emitter>();
    componentRegistry->RegisterComponent<GpuParticleEmitter>();
    componentRegistry->RegisterComponent<DissolveEffectParam>();
    componentRegistry->RegisterComponent<DistortionEffectParam>();
    componentRegistry->RegisterComponent<RadialBlurParam>();
    componentRegistry->RegisterComponent<RandomEffectParam>();
    componentRegistry->RegisterComponent<VignetteParam>();
    componentRegistry->RegisterComponent<SpeedlineEffectParam>();
    componentRegistry->RegisterComponent<MaterialEffectPipeLine>();
    componentRegistry->RegisterComponent<GradationComponent>();

    componentRegistry->RegisterComponent<ModelMeshRenderer>();
    componentRegistry->RegisterComponent<LineRenderer>();
    componentRegistry->RegisterComponent<PlaneRenderer>();
    componentRegistry->RegisterComponent<RingRenderer>();
    componentRegistry->RegisterComponent<BoxRenderer>();
    componentRegistry->RegisterComponent<SphereRenderer>();
    componentRegistry->RegisterComponent<SkyboxRenderer>();
    componentRegistry->RegisterComponent<SpriteRenderer>();
    componentRegistry->RegisterComponent<CylinderRenderer>();

    componentRegistry->RegisterComponent<TimerComponent>();
    componentRegistry->RegisterComponent<TimerForSpriteComponent>();

    componentRegistry->RegisterComponent<SpeedFor3dUIComponent>();

    componentRegistry->RegisterComponent<CameraController>();
    componentRegistry->RegisterComponent<CameraShakeSourceComponent>();
    componentRegistry->RegisterComponent<CameraMotionBob>();

    componentRegistry->RegisterComponent<PlayerInput>();
    componentRegistry->RegisterComponent<PlayerStatus>();
    componentRegistry->RegisterComponent<PlayerState>();
    componentRegistry->RegisterComponent<PlayerEffectControlParam>();

    componentRegistry->RegisterComponent<Button>();
    componentRegistry->RegisterComponent<ButtonGroup>();
    componentRegistry->RegisterComponent<SceneChanger>();

    componentRegistry->RegisterComponent<PlayerStateOverrideCondition>();
}

void RegisterUsingSystems() {
    SystemRegistry* systemRegistry = SystemRegistry::GetInstance();

    /// ====================================================================================================
    // Initialize
    /// ====================================================================================================
    systemRegistry->RegisterSystem<SettingGameCameraTarget>();
    systemRegistry->RegisterSystem<GpuParticleInitialize>();
    systemRegistry->RegisterSystem<ResolveEntityReferences>();
    systemRegistry->RegisterSystem<CreateSpriteFromTimer>();
    systemRegistry->RegisterSystem<Ui3dObjectInitialize>();
    systemRegistry->RegisterSystem<GetClearTime>();
    systemRegistry->RegisterSystem<CameraInitialize>();
    systemRegistry->RegisterSystem<RegisterWindowResizeEvent>();
    systemRegistry->RegisterSystem<InitializeMouseCondition>();

    systemRegistry->RegisterSystem<StartTimerInitialize>();

    systemRegistry->RegisterSystem<BackFireInitialize>();

    systemRegistry->RegisterSystem<SelectPreviewSceneInitialize>();

    systemRegistry->RegisterSystem<TakePlayerToStartPosition>();
    systemRegistry->RegisterSystem<TakeToGoalPosition>();
    systemRegistry->RegisterSystem<CreatePlaneFromSpeed>();

    systemRegistry->RegisterSystem<GhostInitializeSystem>();
    systemRegistry->RegisterSystem<PlayRecorderInitialize>();

    /// ===================================================================================================
    // Input
    /// ===================================================================================================
    systemRegistry->RegisterSystem<ButtonInputSystem>();
    systemRegistry->RegisterSystem<CameraInputSystem>();
    systemRegistry->RegisterSystem<PlayerInputSystem>();

    systemRegistry->RegisterSystem<PlayRecordSystem>();
    systemRegistry->RegisterSystem<GhostInputUpdate>();

    /// ===================================================================================================
    // StateTransition
    /// ===================================================================================================
    systemRegistry->RegisterSystem<EffectAutoDestroySystem>();
    systemRegistry->RegisterSystem<ChangeSceneByButton>();
    systemRegistry->RegisterSystem<FallDetectionSystem>();
    systemRegistry->RegisterSystem<SceneTransition>();
    systemRegistry->RegisterSystem<TransitionPlayerState>();
    systemRegistry->RegisterSystem<UpdateButtonColorByState>();
    systemRegistry->RegisterSystem<ButtonGroupSystem>();
    systemRegistry->RegisterSystem<TimerCountDown>();
    systemRegistry->RegisterSystem<TimeLimitJudgeSystem>();
    systemRegistry->RegisterSystem<ShowGameUIByInputDevice>();
    systemRegistry->RegisterSystem<ApplyMouseConditionSystem>();
    systemRegistry->RegisterSystem<SubSceneActivateByButton>();
    systemRegistry->RegisterSystem<SubSceneDeactivateByButton>();
    systemRegistry->RegisterSystem<ExitApplicationByButton>();
    systemRegistry->RegisterSystem<PenaltySystem>();
    systemRegistry->RegisterSystem<CameraMotionBobSystem>();

    systemRegistry->RegisterSystem<GameFailedSceneLaunchSystem>();

    systemRegistry->RegisterSystem<RestartOnButtonPressSystem>();

    systemRegistry->RegisterSystem<ButtonScenePreviewSystem>();

    systemRegistry->RegisterSystem<PlayerStateOverrideSystem>();

    /// =================================================================================================
    // Movement
    /// =================================================================================================
    systemRegistry->RegisterSystem<BillboardTransform>();

    systemRegistry->RegisterSystem<MoveSystemByRigidBody>();
    systemRegistry->RegisterSystem<FollowCameraUpdateSystem>();
    systemRegistry->RegisterSystem<PlayerMoveSystem>();

    systemRegistry->RegisterSystem<PlayerPathSplineGenerator>();
    systemRegistry->RegisterSystem<TireTrailGenerateSystem>();
    systemRegistry->RegisterSystem<PlayerUpdateOnTitle>();

    systemRegistry->RegisterSystem<ShadowPlaneSyncPlayerPosition>();

    systemRegistry->RegisterSystem<StartSequenceSystem>();

    systemRegistry->RegisterSystem<SceneTransitionRequestSenderSystem>();
    systemRegistry->RegisterSystem<SceneTransitionRequestReceiverSystem>();

    systemRegistry->RegisterSystem<SubSceneUpdate>();
    systemRegistry->RegisterSystem<RestartSystem>();
    systemRegistry->RegisterSystem<PauseMainSceneSystem>();

    systemRegistry->RegisterSystem<LookAtFromTransformsSystem>();
    systemRegistry->RegisterSystem<Ui3dUpdateSystem>();

    /// =================================================================================================
    // Collision
    /// =================================================================================================
    systemRegistry->RegisterSystem<CollisionCheckSystem>();
    systemRegistry->RegisterSystem<CollisionPushBackSystem>();

    systemRegistry->RegisterSystem<PlayerOnCollision>();
    systemRegistry->RegisterSystem<TutorialColliderOnCollision>();

    systemRegistry->RegisterSystem<ShadowCastSystem>();

    /// =================================================================================================
    // Effect
    /// =================================================================================================
    systemRegistry->RegisterSystem<EmitterWorkSystem>();
    systemRegistry->RegisterSystem<GpuParticleEmitterWorkSystem>();

    systemRegistry->RegisterSystem<PrimitiveNodeAnimationWorkSystem>();
    systemRegistry->RegisterSystem<SkinningAnimationSystem>();
    systemRegistry->RegisterSystem<SpriteAnimationSystem>();
    systemRegistry->RegisterSystem<MaterialAnimationWorkSystem>();
    systemRegistry->RegisterSystem<CameraActionSystem>();
    systemRegistry->RegisterSystem<TransformAnimationWorkSystem>();

    systemRegistry->RegisterSystem<CameraShake>();

    systemRegistry->RegisterSystem<MaterialEffect>();
    systemRegistry->RegisterSystem<CreateMeshFromSpline>();
    systemRegistry->RegisterSystem<CreateMeshFromTireSpline>();

    systemRegistry->RegisterSystem<PlayerSpeedFor3dUI>();
    systemRegistry->RegisterSystem<TimerForSprite>();

    systemRegistry->RegisterSystem<EffectOnPlayerGearup>();
    systemRegistry->RegisterSystem<EffectOnPlayerRun>();

    systemRegistry->RegisterSystem<PenaltyTimeSpriteUpdate>();

    /// =================================================================================================
    // Render
    /// =================================================================================================
    systemRegistry->RegisterSystem<SkyboxRender>();
    systemRegistry->RegisterSystem<SpriteRenderSystem>();
    systemRegistry->RegisterSystem<BackGroundSpriteRenderSystem>();
    systemRegistry->RegisterSystem<ParticleRenderSystem>();
    systemRegistry->RegisterSystem<TexturedMeshRenderSystem>();
    systemRegistry->RegisterSystem<OverlayRenderSystem>();
    systemRegistry->RegisterSystem<SkinningMeshRenderSystem>();
    systemRegistry->RegisterSystem<LineRenderSystem>();
    systemRegistry->RegisterSystem<GpuParticleRenderSystem>();

#ifndef _RELEASE
    systemRegistry->RegisterSystem<SkeletonRenderSystem>();
    systemRegistry->RegisterSystem<ColliderRenderingSystem>();
    systemRegistry->RegisterSystem<VelocityRenderingSystem>();
#endif // _RELEASE

    /// =================================================================================================
    // PostRender
    /// =================================================================================================
    systemRegistry->RegisterSystem<GrayscaleEffect>();
    systemRegistry->RegisterSystem<SmoothingEffect>();
    systemRegistry->RegisterSystem<VignetteEffect>();
    systemRegistry->RegisterSystem<DistortionEffect>();
    systemRegistry->RegisterSystem<DissolveEffect>();
    systemRegistry->RegisterSystem<RandomEffect>();
    systemRegistry->RegisterSystem<RadialBlurEffect>();
    systemRegistry->RegisterSystem<SubSceneRender>();
    systemRegistry->RegisterSystem<SpeedlineEffect>();
    systemRegistry->RegisterSystem<GradationEffect>();
}
