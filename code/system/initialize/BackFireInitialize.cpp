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
    OriGine::Entity* dissEntity      = GetUniqueEntity("BackFire_Diss");
    OriGine::Entity* distEntity      = GetUniqueEntity("BackFire_Dist");
    OriGine::Entity* gradationEntity = GetUniqueEntity("BackFire_Grad");
    OriGine::Entity* sparks          = GetUniqueEntity("BackFireSparks");
    OriGine::Entity* sparksDiss      = GetUniqueEntity("BackFireSparks_Diss");

    /// backFireのエフェクトを構築
    MaterialEffectPipeLine* effectPipeline = GetComponent<MaterialEffectPipeLine>(_entity);
    effectPipeline->ClearEffectEntity();

    MaterialEffectPipeLine* dissPipeline = GetComponent<MaterialEffectPipeLine>(dissEntity);
    dissPipeline->AddEffectEntity(MaterialEffectType::Distortion, distEntity->GetID());

    effectPipeline->AddEffectEntity(MaterialEffectType::Dissolve, dissEntity->GetID());
    effectPipeline->AddEffectEntity(MaterialEffectType::Gradation, gradationEntity->GetID());

    /// sparks の エフェクト構築
    MaterialEffectPipeLine* sparkPipeline = GetComponent<MaterialEffectPipeLine>(sparks);

    MaterialEffectPipeLine* sparksDissPipeline = GetComponent<MaterialEffectPipeLine>(sparksDiss);
    sparksDissPipeline->AddEffectEntity(MaterialEffectType::Distortion, distEntity->GetID());

    sparkPipeline->AddEffectEntity(MaterialEffectType::Dissolve, sparksDiss->GetID());
    sparkPipeline->AddEffectEntity(MaterialEffectType::Gradation, gradationEntity->GetID());

    /// Transform
    Transform* playerTransform = GetComponent<OriGine::Transform>(GetUniqueEntity("Player"));
    Transform* backFirTrans    = GetComponent<OriGine::Transform>(_entity);
    Transform* sparksTrans     = GetComponent<OriGine::Transform>(sparks);
    backFirTrans->parent       = playerTransform;
    sparksTrans->parent        = backFirTrans;
}
