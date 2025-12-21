#include "SceneTransition.h"

/// engine
#include "Engine.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"

/// ECS
// component
#include "component/effect/post/DissolveEffectParam.h"
#include "component/SceneChanger.h"

/// math
#include "math/MyEasing.h"

using namespace OriGine;

SceneTransition::SceneTransition() : ISystem(OriGine::SystemCategory::StateTransition) {}
SceneTransition::~SceneTransition() {}

void SceneTransition::Initialize() {
    enterScene_ = true;
    exitScene_  = false;

    currentTransitionTime_ = maxTransitionTime_;
}

void SceneTransition::Finalize() {
    enterScene_              = false;
    exitScene_               = false;
    usingEntityId_           = -1;
    sceneChangerComponentId_ = -1;

    currentTransitionTime_ = 0.f;
}

void SceneTransition::Update() {
#ifdef _DEBUG
    // 計測開始
    deltaTimer_.Initialize();
#endif

    if (entityIDs_.empty()) {
        return;
    }

    EraseDeadEntity();

    // シーンに入るエフェクトの更新
    if (enterScene_) {
        auto* enterSceneEntity = GetUniqueEntity("EnterScene");
        if (!enterSceneEntity) {
            enterScene_ = false;
            return;
        }
        usingEntityId_ = enterSceneEntity->GetID();

        EnterSceneUpdate();
    } else if (exitScene_) { // シーンから出るエフェクトの更新
        ExitSceneUpdate();
    } else {
        for (auto& entityID : entityIDs_) {
            OriGine::Entity* entity = GetEntity(entityID);
            UpdateEntity(entity);
        }
    }

#ifdef _DEBUG
    // 計測終了
    deltaTimer_.Update();
#endif
}

void SceneTransition::UpdateEntity(OriGine::EntityHandle _handle) {
    uint32_t compSize = (uint32_t)GetComponentArray<SceneChanger>()->GetComponentSize(_entity);
    if (compSize <= 0) {
        return;
    }

    // シーン遷移の検知
    for (uint32_t i = 0; i < compSize; ++i) {
        SceneChanger* sceneChanger = GetComponent<SceneChanger>(_entity, i);
        if (sceneChanger == nullptr) {
            continue;
        }
        if (sceneChanger->isChanged()) {

            currentTransitionTime_   = 0.f;
            exitScene_               = true;
            enterScene_              = false;
            usingEntityId_           = _entity->GetID();
            sceneChangerComponentId_ = i;
            return;
        }
    }
}

void SceneTransition::EnterSceneUpdate() {

    OriGine::Entity* enterSceneEntity = GetEntity(usingEntityId_);
    if (enterSceneEntity == nullptr) {
        return;
    }

    currentTransitionTime_ -= Engine::GetInstance()->GetDeltaTime();

    ///====================================================================
    /// 遷移時間が0以下になった場合、再生終了
    ///====================================================================
    if (currentTransitionTime_ <= 0.0f) {
        currentTransitionTime_ = 0.0f;
        enterScene_            = false;
        exitScene_             = false;

        uint32_t compSize = (uint32_t)GetComponentArray<DissolveEffectParam>()->GetComponentSize(enterSceneEntity);
        if (compSize <= 0) {
            return;
        }

        for (uint32_t i = 0; i < compSize; ++i) {
            DissolveEffectParam* dissolveEffectParam = GetComponent<DissolveEffectParam>(enterSceneEntity, i);
            if (dissolveEffectParam == nullptr) {
                return;
            }
            dissolveEffectParam->Stop();
        }

        return;
    }

    uint32_t compSize = (uint32_t)GetComponentArray<DissolveEffectParam>()->GetComponentSize(enterSceneEntity);
    if (compSize <= 0) {
        return;
    }

    for (uint32_t i = 0; i < compSize; ++i) {
        DissolveEffectParam* dissolveEffectParam = GetComponent<DissolveEffectParam>(enterSceneEntity, i);
        if (dissolveEffectParam == nullptr) {
            return;
        }
        if (!dissolveEffectParam->IsActive()) {
            dissolveEffectParam->Play();
        }
        dissolveEffectParam->SetThreshold(EaseInQuad(currentTransitionTime_ / maxTransitionTime_));
    }
}

void SceneTransition::ExitSceneUpdate() {
    currentTransitionTime_ += Engine::GetInstance()->GetDeltaTime();
    if (currentTransitionTime_ >= maxTransitionTime_) {
        currentTransitionTime_ = maxTransitionTime_;
        enterScene_            = false;
        exitScene_             = false;

        // シーン変更を実行
        OriGine::Entity* sceneChangerEntity = GetEntity(usingEntityId_);
        SceneChanger* sceneChanger          = GetComponent<SceneChanger>(sceneChangerEntity, sceneChangerComponentId_);
        GetScene()->GetSceneManager()->ChangeScene(sceneChanger->GetNextSceneName());
    }

    OriGine::Entity* enterSceneEntity = GetEntity(usingEntityId_);
    if (enterSceneEntity == nullptr) {
        return;
    }

    uint32_t compSize = (uint32_t)GetComponentArray<DissolveEffectParam>()->GetComponentSize(enterSceneEntity);
    if (compSize <= 0) {
        return;
    }

    for (uint32_t i = 0; i < compSize; ++i) {
        DissolveEffectParam* dissolveEffectParam = GetComponent<DissolveEffectParam>(enterSceneEntity, i);
        if (dissolveEffectParam == nullptr) {
            return;
        }
        if (!dissolveEffectParam->IsActive()) {
            dissolveEffectParam->Play();
        }
        dissolveEffectParam->SetThreshold(EaseInQuad(currentTransitionTime_ / maxTransitionTime_));
    }
}
