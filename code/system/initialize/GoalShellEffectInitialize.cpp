#include "GoalShellEffectInitialize.h"

/// component
#include "component/effect/MaterialEffectPipeLine.h"
#include "component/transform/Transform.h"

GoalShellEffectInitialize::GoalShellEffectInitialize() : ISystem(SystemCategory::Initialize) {}
GoalShellEffectInitialize::~GoalShellEffectInitialize() {}

void GoalShellEffectInitialize::Initialize() {
    // 何もしない
}
void GoalShellEffectInitialize::Finalize() {
    // 何もしない
}

void GoalShellEffectInitialize::UpdateEntity(Entity* entity) {
    // 移動
    Transform* transform = GetComponent<Transform>(entity);
    Entity* goalEntity   = GetUniqueEntity("Goal");
    if (goalEntity) {
        Transform* goalTransform = GetComponent<Transform>(goalEntity);
        if (transform && goalTransform) {
            transform->translate = goalTransform->translate;
            transform->UpdateMatrix();
        }
    }

    // エフェクト準備 (pipelineの構築)
    auto materialEffectPipeLine = GetComponent<MaterialEffectPipeLine>(entity);
    if (materialEffectPipeLine == nullptr) {
        return;
    }

    // 下準備 DissolveにDistortionを追加する
    Entity* dissEffectEntity = GetUniqueEntity("GoalShellEffectDiss");
    Entity* distEffectEntity = GetUniqueEntity("GoalShellEffectDist");

    if (!dissEffectEntity || !distEffectEntity) {
        return;
    }

    auto dissEffectMaterialPipeline = GetComponent<MaterialEffectPipeLine>(dissEffectEntity);
    if (!dissEffectMaterialPipeline) {
        return;
    }

    // Distortionエフェクトを追加
    dissEffectMaterialPipeline->AddEffectEntity(MaterialEffectType::Distortion, distEffectEntity->GetID());

    // 最後に Dissolveエフェクトをmainに追加
    materialEffectPipeLine->AddEffectEntity(MaterialEffectType::Dissolve, dissEffectEntity->GetID());
}
