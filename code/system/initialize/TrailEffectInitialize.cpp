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
        getComponent<MaterialEffectPipeLine>(_entity);

    if (materialEffectPipeLine == nullptr) {
        return;
    }
    materialEffectPipeLine->ClearEffectEntity();

    // dissolveEffect 設定
    auto dissolveEntity = getUniqueEntity("trailDissolve");
    if (!dissolveEntity) {
        LOG_ERROR("dissolveEntity is nullptr");
        return;
    }

    auto dissolveMaterialEffectPipeline =
        getComponent<MaterialEffectPipeLine>(dissolveEntity);
    if (!dissolveMaterialEffectPipeline) {
        return;
    }
    dissolveMaterialEffectPipeline->ClearEffectEntity();

    // dissolveに distortionEffect1を 設定
    auto distortion1Entity = getUniqueEntity("trailDistortion1Entity");
    if (!distortion1Entity) {
        LOG_ERROR("distortion1Entity is nullptr");
        return;
    }
    auto distortion1EffectParam =
        getComponent<DistortionEffectParam>(distortion1Entity);
    if (distortion1EffectParam) {
        dissolveMaterialEffectPipeline->AddEffectEntity(
            MaterialEffectType::Distortion, distortion1Entity->getID());
    }

    // trailに dissolveEffectを 設定
    if (dissolveMaterialEffectPipeline) {
        materialEffectPipeLine->AddEffectEntity(
            MaterialEffectType::Dissolve, dissolveEntity->getID());

        auto gradationEntity = getUniqueEntity("trailGradationEntity");
        if (!gradationEntity) {
            LOG_ERROR("gradationEntity is nullptr");
            return;
        }
        auto gradationTextureComponent =
            getComponent<GradationTextureComponent>(gradationEntity);
        // trailに Gradationを設定
        if (gradationTextureComponent) {
            materialEffectPipeLine->AddEffectEntity(
                MaterialEffectType::Gradation, gradationEntity->getID());
        }
    }
    materialEffectPipeLine->setPriority(10);
}
