#include "BackFireInitialize.h"

/// component
#include "component/effect/MaterialEffectPipeLine.h"
#include "component/player/PlayerStatus.h"
#include "component/renderer/MeshRenderer.h"

using namespace OriGine;

BackFireInitialize::BackFireInitialize() : ISystem(OriGine::SystemCategory::Initialize) {}
BackFireInitialize::~BackFireInitialize() {}

void BackFireInitialize::Initialize() {}
void BackFireInitialize::Finalize() {}

void BackFireInitialize::UpdateEntity(OriGine::EntityHandle _handle) {
    OriGine::EntityHandle dissEntity      = GetUniqueEntity("BackFire_Diss");
    OriGine::EntityHandle distEntity      = GetUniqueEntity("BackFire_Dist");
    OriGine::EntityHandle gradationEntity = GetUniqueEntity("BackFire_Grad");
    OriGine::EntityHandle sparks          = GetUniqueEntity("BackFireSparks");
    OriGine::EntityHandle sparksDiss      = GetUniqueEntity("BackFireSparks_Diss");

    /// backFireのエフェクトを構築
    MaterialEffectPipeLine* effectPipeline = GetComponent<MaterialEffectPipeLine>(_handle);
    effectPipeline->ClearEffectEntity();

    MaterialEffectPipeLine* dissPipeline = GetComponent<MaterialEffectPipeLine>(dissEntity);
    dissPipeline->AddEffectEntity(MaterialEffectType::Distortion, dissEntity);

    effectPipeline->AddEffectEntity(MaterialEffectType::Dissolve, dissEntity);
    effectPipeline->AddEffectEntity(MaterialEffectType::Gradation, gradationEntity);

    /// sparks の エフェクト構築
    MaterialEffectPipeLine* sparkPipeline = GetComponent<MaterialEffectPipeLine>(sparks);

    MaterialEffectPipeLine* sparksDissPipeline = GetComponent<MaterialEffectPipeLine>(sparksDiss);
    sparksDissPipeline->AddEffectEntity(MaterialEffectType::Distortion, distEntity);

    sparkPipeline->AddEffectEntity(MaterialEffectType::Dissolve, sparksDiss);
    sparkPipeline->AddEffectEntity(MaterialEffectType::Gradation, gradationEntity);

    /// Transform
    Transform* playerTransform = GetComponent<OriGine::Transform>(GetUniqueEntity("Player"));
    Transform* backFirTrans    = GetComponent<OriGine::Transform>(_handle);
    Transform* sparksTrans     = GetComponent<OriGine::Transform>(sparks);
    backFirTrans->parent       = playerTransform;
    sparksTrans->parent        = backFirTrans;
}
