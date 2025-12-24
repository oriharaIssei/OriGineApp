#include "TrailEffectInitialize.h"

// component
#include "component/effect/MaterialEffectPipeLine.h"
#include "component/effect/post/DissolveEffectParam.h"
#include "component/effect/post/DistortionEffectParam.h"
#include "component/effect/post/GradationComponent.h"

/// externals
#include "logger/Logger.h"

using namespace OriGine;

TrailEffectInitialize::TrailEffectInitialize() : ISystem(OriGine::SystemCategory::Initialize) {}
TrailEffectInitialize::~TrailEffectInitialize() {}

void TrailEffectInitialize::Initialize() {}
void TrailEffectInitialize::Finalize() {}

void TrailEffectInitialize::UpdateEntity(OriGine::EntityHandle _handle) {
    MaterialEffectPipeLine* materialEffectPipeLine =
        GetComponent<MaterialEffectPipeLine>(_handle);

    if (materialEffectPipeLine == nullptr) {
        return;
    }
    materialEffectPipeLine->ClearEffectEntity();

    // dissolveEffect 設定
    auto dissolveEntity = GetUniqueEntity("trailDissolve");
    if (!dissolveEntity.IsValid()) {
        LOG_ERROR("dissolveEntity is nullptr");
        return;
    }

    auto dissolveMaterialEffectPipeline =
        GetComponent<MaterialEffectPipeLine>(dissolveEntity);
    if (!dissolveMaterialEffectPipeline) {
        return;
    }
    dissolveMaterialEffectPipeline->ClearEffectEntity();

    // dissolveに distortionEffect1を 設定
    auto distortion1EntityHandle = GetUniqueEntity("trailDistortion1Entity");
    if (!distortion1EntityHandle.IsValid()) {
        LOG_ERROR("distortion1Entity is nullptr");
        return;
    }
    auto distortion1EffectParam =
        GetComponent<DistortionEffectParam>(distortion1EntityHandle);
    if (distortion1EffectParam) {
        dissolveMaterialEffectPipeline->AddEffectEntity(
            MaterialEffectType::Distortion, distortion1EffectParam->GetHandle());
    }

    // trailに dissolveEffectを 設定
    if (dissolveMaterialEffectPipeline) {
        auto dissolveEffectParam =
            GetComponent<DissolveEffectParam>(dissolveEntity);
        if (!dissolveEffectParam) {
            LOG_ERROR("trailDissolve is nullptr");
            return;
        }
        materialEffectPipeLine->AddEffectEntity(
            MaterialEffectType::Dissolve, dissolveEffectParam->GetHandle());

        auto gradationEntity = GetUniqueEntity("trailGradationEntity");
        if (!gradationEntity.IsValid()) {
            LOG_ERROR("gradationEntity is nullptr");
            return;
        }
        auto gradationTextureComponent =
            GetComponent<GradationComponent>(gradationEntity);
        // trailに Gradationを設定
        if (gradationTextureComponent) {
            materialEffectPipeLine->AddEffectEntity(
                MaterialEffectType::Gradation, gradationTextureComponent->GetHandle());
        }
    }
    materialEffectPipeLine->SetPriority(10);
}
