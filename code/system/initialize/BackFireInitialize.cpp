#include "BackFireInitialize.h"

/// component
#include "component/effect/MaterialEffectPipeLine.h"
#include "component/player/PlayerStatus.h"
#include "component/renderer/MeshRenderer.h"

BackFireInitialize::BackFireInitialize() : ISystem(SystemCategory::Initialize) {}
BackFireInitialize::~BackFireInitialize() {}

void BackFireInitialize::Initialize() {}
void BackFireInitialize::Finalize() {}

void BackFireInitialize::UpdateEntity(Entity* _entity) {
    Entity* dissEntity      = GetUniqueEntity("BackFire_Diss");
    Entity* distEntity      = GetUniqueEntity("BackFire_Dist");
    Entity* gradationEntity = GetUniqueEntity("BackFire_Grad");
    Entity* sparks          = GetUniqueEntity("BackFireSparks");
    Entity* sparksDiss      = GetUniqueEntity("BackFireSparks_Diss");

    /// backFireのエフェクトを構築
    MaterialEffectPipeLine* effectPipeline = GetComponent<MaterialEffectPipeLine>(_entity);

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
    Transform* playerTransform = GetComponent<Transform>(GetUniqueEntity("Player"));
    Transform* backFirTrans    = GetComponent<Transform>(_entity);
    Transform* sparksTrans     = GetComponent<Transform>(sparks);
    backFirTrans->parent       = playerTransform;
    sparksTrans->parent        = backFirTrans;
}
