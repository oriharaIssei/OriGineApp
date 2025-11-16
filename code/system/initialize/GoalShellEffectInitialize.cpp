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
    Transform* transform = getComponent<Transform>(entity);
    Entity* goalEntity   = getUniqueEntity("Goal");
    if (goalEntity) {
        Transform* goalTransform = getComponent<Transform>(goalEntity);
        if (transform && goalTransform) {
            transform->translate = goalTransform->translate;
            transform->UpdateMatrix();
        }
    }

    // エフェクト準備 (pipelineの構築)
    auto materialEffectPipeLine = getComponent<MaterialEffectPipeLine>(entity);
    if (materialEffectPipeLine == nullptr) {
        return;
    }

    // 下準備 DissolveにDistortionを追加する
    Entity* dissEffectEntity = getUniqueEntity("GoalShellEffectDiss");
    Entity* distEffectEntity = getUniqueEntity("GoalShellEffectDist");

    if (!dissEffectEntity || !distEffectEntity) {
        return;
    }

    auto dissEffectMaterialPipeline = getComponent<MaterialEffectPipeLine>(dissEffectEntity);
    if (!dissEffectMaterialPipeline) {
        return;
    }

    // Distortionエフェクトを追加
    dissEffectMaterialPipeline->AddEffectEntity(MaterialEffectType::Distortion, distEffectEntity->getID());

    // 最後に Dissolveエフェクトをmainに追加
    materialEffectPipeLine->AddEffectEntity(MaterialEffectType::Dissolve, dissEffectEntity->getID());
}
