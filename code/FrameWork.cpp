#include "FrameWork.h"

/// engine include
#define ENGINE_INCLUDE
#define ENGINE_ECS
#define ENGINE_COMPONENTS
#define ENGINE_SYSTEMS
#include <EngineInclude.h>

// application component
#include "component/Button.h"
#include "component/ButtonGroup.h"
#include "component/Camera/CameraController.h"
#include "component/camera/CameraMotionBob.h"
#include "component/Camera/CameraShakeSourceComponent.h"
#include "component/LookAtFromTransforms.h"
#include "component/MouseCondition.h"
#include "component/player/PlayerEffectControlParam.h"
#include "component/player/PlayerInput.h"
#include "component/player/PlayerStatus.h"
#include "component/player/state/PlayerState.h"
#include "component/SceneChanger.h"
#include "component/spline/SplinePoints.h"
#include "component/stage/StageData.h"
#include "component/stage/StageObstacle.h"
#include "component/TimerComponent.h"

// application system
#include "system/collision/PlayerOnCollision.h"
#include "system/collision/TutorialColliderOnCollision.h"
#include "system/effect/CameraShake.h"
#include "system/effect/CreateMeshFromSpline.h"
#include "system/effect/EffectOnPlayerGearup.h"
#include "system/effect/EffectOnPlayerRun.h"
#include "system/effect/PenaltyTimeSpriteUpdate.h"
#include "system/effect/TimerForSprite.h"
#include "system/initialize/BackFireInitialize.h"
#include "system/initialize/CreateSpriteFromTimer.h"
#include "system/initialize/CreateStage.h"
#include "system/initialize/GetClearTime.h"
#include "system/initialize/SettingGameCameraTarget.h"
#include "system/initialize/StartTimerInitialize.h"
#include "system/initialize/TakePlayerToStartPosition.h"
#include "system/initialize/TakeToGoalPosition.h"
#include "system/initialize/TrailEffectInitialize.h"
#include "system/Initialize/Ui3dObjectInitialize.h"
#include "system/input/ButtonInputSystem.h"
#include "system/input/CameraInputSystem.h"
#include "system/input/PlayerInputSystem.h"
#include "system/movement/BillboardTransform.h"
#include "system/movement/FollowCameraUpdateSystem.h"
#include "system/Movement/LookAtFromTransformsSystem.h"
#include "system/movement/MenuUpdate.h"
#include "system/movement/PlayerMoveSystem.h"
#include "system/movement/PlayerPathSplineGenerator.h"
#include "system/movement/PlayerUpdateOnTitle.h"
#include "system/movement/StartSequenceSystem.h"
#include "system/Movement/Ui3dUpdateSystem.h"
#include "system/transition/ApplyMouseConditionSystem.h"
#include "system/transition/ButtonGroupSystem.h"
#include "system/transition/ButtonScenePreviewSystem.h"
#include "system/Transition/CameraMotionBobSystem.h"
#include "system/transition/ChangeSceneByButton.h"
#include "system/transition/ExitApplicationByButton.h"
#include "system/transition/FallDetectionSystem.h"
#include "system/transition/PenaltySystem.h"
#include "system/transition/SceneTransition.h"
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
    ComponentRegistry* componentRegistry = ComponentRegistry::GetInstance();

    componentRegistry->RegisterComponent<Audio>();

    componentRegistry->RegisterComponent<MouseCondition>();

    componentRegistry->RegisterComponent<EntityReferenceList>();
    componentRegistry->RegisterComponent<SubScene>();

    componentRegistry->RegisterComponent<CameraTransform>();
    componentRegistry->RegisterComponent<Transform>();

    componentRegistry->RegisterComponent<LookAtFromTransforms>();

    componentRegistry->RegisterComponent<StageData>();
    componentRegistry->RegisterComponent<StageObstacle>();

    componentRegistry->RegisterComponent<SplinePoints>();

    componentRegistry->RegisterComponent<Material>();
    componentRegistry->RegisterComponent<DirectionalLight>();
    componentRegistry->RegisterComponent<PointLight>();
    componentRegistry->RegisterComponent<SpotLight>();

    componentRegistry->RegisterComponent<CameraAction>();
    componentRegistry->RegisterComponent<ModelNodeAnimation>();
    componentRegistry->RegisterComponent<PrimitiveNodeAnimation>();
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
}

void RegisterUsingSystems() {
    SystemRegistry* systemRegistry = SystemRegistry::GetInstance();

    /// ====================================================================================================
    // Initialize
    /// ====================================================================================================
    systemRegistry->RegisterSystem<SettingGameCameraTarget>();
    systemRegistry->RegisterSystem<GpuParticleInitialize>();
    systemRegistry->RegisterSystem<CreateStage>();
    systemRegistry->RegisterSystem<ResolveEntityReferences>();
    systemRegistry->RegisterSystem<CreateSpriteFromTimer>();
    systemRegistry->RegisterSystem<GetClearTime>();
    systemRegistry->RegisterSystem<CameraInitialize>();
    systemRegistry->RegisterSystem<Ui3dObjectInitialize>();
    systemRegistry->RegisterSystem<RegisterWindowResizeEvent>();
    systemRegistry->RegisterSystem<TrailEffectInitialize>();

    systemRegistry->RegisterSystem<StartTimerInitialize>();

    systemRegistry->RegisterSystem<BackFireInitialize>();

    systemRegistry->RegisterSystem<TakePlayerToStartPosition>();
    systemRegistry->RegisterSystem<TakeToGoalPosition>();

    /// ===================================================================================================
    // Input
    /// ===================================================================================================
    systemRegistry->RegisterSystem<ButtonInputSystem>();
    systemRegistry->RegisterSystem<CameraInputSystem>();
    systemRegistry->RegisterSystem<PlayerInputSystem>();

    /// ===================================================================================================
    // StateTransition
    /// ===================================================================================================
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

    systemRegistry->RegisterSystem<ButtonScenePreviewSystem>();

    /// =================================================================================================
    // Movement
    /// =================================================================================================
    systemRegistry->RegisterSystem<BillboardTransform>();

    systemRegistry->RegisterSystem<MoveSystemByRigidBody>();
    systemRegistry->RegisterSystem<FollowCameraUpdateSystem>();
    systemRegistry->RegisterSystem<PlayerMoveSystem>();

    systemRegistry->RegisterSystem<PlayerUpdateOnTitle>();

    systemRegistry->RegisterSystem<PlayerPathSplineGenerator>();

    systemRegistry->RegisterSystem<StartSequenceSystem>();

    systemRegistry->RegisterSystem<SubSceneUpdate>();
    systemRegistry->RegisterSystem<MenuUpdate>();

    systemRegistry->RegisterSystem<LookAtFromTransformsSystem>();
    systemRegistry->RegisterSystem<Ui3dUpdateSystem>();

    /// =================================================================================================
    // Collision
    /// =================================================================================================
    systemRegistry->RegisterSystem<CollisionCheckSystem>();
    systemRegistry->RegisterSystem<CollisionPushBackSystem>();

    systemRegistry->RegisterSystem<PlayerOnCollision>();
    systemRegistry->RegisterSystem<TutorialColliderOnCollision>();

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
    systemRegistry->RegisterSystem<CameraShake>();

    systemRegistry->RegisterSystem<MaterialEffect>();
    systemRegistry->RegisterSystem<CreateMeshFromSpline>();

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

    systemRegistry->RegisterSystem<TimerForSprite>();

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
