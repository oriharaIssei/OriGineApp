#include "TrailEffectInitialize.h"

// component
#include "component/effect/MaterialEffectPipeLine.h"
#include "component/effect/post/DissolveEffectParam.h"
#include "component/effect/post/DistortionEffectParam.h"
#include "component/effect/post/GradationTextureComponent.h"

/// externals
#include "logger/Logger.h"

TrailEffectInitialize::TrailEffectInitialize() : ISystem(SystemCategory::Initialize) {}
TrailEffectInitialize::~TrailEffectInitialize() {}

void TrailEffectInitialize::Initialize() {}
void TrailEffectInitialize::Finalize() {}

void TrailEffectInitialize::UpdateEntity(Entity* _entity) {
    MaterialEffectPipeLine* materialEffectPipeLine =
        GetComponent<MaterialEffectPipeLine>(_entity);

    if (materialEffectPipeLine == nullptr) {
        return;
    }
    materialEffectPipeLine->ClearEffectEntity();

    // dissolveEffect 設定
    auto dissolveEntity = GetUniqueEntity("trailDissolve");
    if (!dissolveEntity) {
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
    auto distortion1Entity = GetUniqueEntity("trailDistortion1Entity");
    if (!distortion1Entity) {
        LOG_ERROR("distortion1Entity is nullptr");
        return;
    }
    auto distortion1EffectParam =
        GetComponent<DistortionEffectParam>(distortion1Entity);
    if (distortion1EffectParam) {
        dissolveMaterialEffectPipeline->AddEffectEntity(
            MaterialEffectType::Distortion, distortion1Entity->GetID());
    }

    // trailに dissolveEffectを 設定
    if (dissolveMaterialEffectPipeline) {
        materialEffectPipeLine->AddEffectEntity(
            MaterialEffectType::Dissolve, dissolveEntity->GetID());

        auto gradationEntity = GetUniqueEntity("trailGradationEntity");
        if (!gradationEntity) {
            LOG_ERROR("gradationEntity is nullptr");
            return;
        }
        auto gradationTextureComponent =
            GetComponent<GradationTextureComponent>(gradationEntity);
        // trailに Gradationを設定
        if (gradationTextureComponent) {
            materialEffectPipeLine->AddEffectEntity(
                MaterialEffectType::Gradation, gradationEntity->GetID());
        }
    }
    materialEffectPipeLine->SetPriority(10);
}
