#include "FrameWork.h"

/// engine include
#define ENGINE_INCLUDE
#define ENGINE_ECS
#define ENGINE_COMPONENTS
#define ENGINE_SYSTEMS
#include <EngineInclude.h>

#include "globalVariables/GlobalVariables.h"

/// =====================================================
// Application Include
/// =====================================================
// component

FrameWork::FrameWork() {}

FrameWork::~FrameWork() {}

//! TODO : 場所 変更 FrameWork はUserに分かりづらすぎる

void RegisterUsingComponents() {
    ComponentRegistry* componentRegistry = ComponentRegistry::getInstance();

    componentRegistry->registerComponent<Transform>();
    componentRegistry->registerComponent<CameraTransform>();

    componentRegistry->registerComponent<DirectionalLight>();
    componentRegistry->registerComponent<PointLight>();
    componentRegistry->registerComponent<SpotLight>();

    componentRegistry->registerComponent<Rigidbody>();

    componentRegistry->registerComponent<AABBCollider>();
    componentRegistry->registerComponent<SphereCollider>();
    componentRegistry->registerComponent<CollisionPushBackInfo>();

    componentRegistry->registerComponent<Emitter>();
    componentRegistry->registerComponent<TextureEffectParam>();
    componentRegistry->registerComponent<VignetteParam>();
    componentRegistry->registerComponent<DistortionEffectParam>();

    componentRegistry->registerComponent<Audio>();

    componentRegistry->registerComponent<ModelNodeAnimation>();
    componentRegistry->registerComponent<PrimitiveNodeAnimation>();

    componentRegistry->registerComponent<ModelMeshRenderer>();
    componentRegistry->registerComponent<PlaneRenderer>();
    componentRegistry->registerComponent<RingRenderer>();
    componentRegistry->registerComponent<SpriteRenderer>();
    componentRegistry->registerComponent<LineRenderer>();
    componentRegistry->registerComponent<SkyboxRenderer>();
}

void RegisterUsingSystems() {
    SystemRegistry* systemRegistry = SystemRegistry::getInstance();

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
    systemRegistry->registerSystem<MoveSystemByRigidBody>();

    /// =================================================================================================
    // Collision
    /// =================================================================================================
    systemRegistry->registerSystem<CollisionCheckSystem>();
    systemRegistry->registerSystem<CollisionPushBackSystem>();

    /// =================================================================================================
    // Effect
    /// =================================================================================================
    systemRegistry->registerSystem<EmitterWorkSystem>();
    systemRegistry->registerSystem<PrimitiveNodeAnimationWorkSystem>();
    systemRegistry->registerSystem<TextureEffectAnimation>();

    /// =================================================================================================
    // Render
    /// =================================================================================================
    systemRegistry->registerSystem<ParticleRenderSystem>();
    systemRegistry->registerSystem<SpriteRenderSystem>();
    systemRegistry->registerSystem<TexturedMeshRenderSystem>();
    systemRegistry->registerSystem<EffectTexturedMeshRenderSystem>();
    systemRegistry->registerSystem<LineRenderSystem>();
    systemRegistry->registerSystem<ColliderRenderingSystem>();
    systemRegistry->registerSystem<SkyboxRender>();
    systemRegistry->registerSystem<BackGroundSpriteRenderSystem>();

    /// =================================================================================================
    // PostRender
    /// =================================================================================================
    systemRegistry->registerSystem<GrayscaleEffect>();
    systemRegistry->registerSystem<SmoothingEffect>();
    systemRegistry->registerSystem<VignetteEffect>();
    systemRegistry->registerSystem<DistortionEffect>();
}
