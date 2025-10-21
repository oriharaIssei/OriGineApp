#include "SceneTransition.h"

/// engine
#include "Engine.h"
#include "scene/SceneManager.h"

/// ECS
// component
#include "component/effect/post/DissolveEffectParam.h"
#include "component/SceneChanger.h"

/// math
#include "math/MyEasing.h"

SceneTransition::SceneTransition() : ISystem(SystemCategory::StateTransition) {}

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

    eraseDeadEntity();

    // シーンに入るエフェクトの更新
    if (enterScene_) {
        auto* enterSceneEntity = getUniqueEntity("EnterScene");
        if (!enterSceneEntity) {
            enterScene_ = false;
            return;
        }
        usingEntityId_ = enterSceneEntity->getID();

        EnterSceneUpdate();
    } else if (exitScene_) { // シーンから出るエフェクトの更新
        ExitSceneUpdate();
    } else {
        for (auto& entityID : entityIDs_) {
            Entity* entity = getEntity(entityID);
            UpdateEntity(entity);
        }
    }

#ifdef _DEBUG
    // 計測終了
    deltaTimer_.Update();
    runningTime_ = deltaTimer_.getDeltaTime();
#endif
}

void SceneTransition::UpdateEntity(Entity* _entity) {
    uint32_t compSize = (uint32_t)getComponentArray<SceneChanger>()->getComponentSize(_entity);
    if (compSize <= 0) {
        return;
    }

    // シーン遷移の検知
    for (uint32_t i = 0; i < compSize; ++i) {
        SceneChanger* sceneChanger = getComponent<SceneChanger>(_entity, i);
        if (sceneChanger == nullptr) {
            continue;
        }
        if (sceneChanger->isChanged()) {

            currentTransitionTime_   = 0.f;
            exitScene_               = true;
            enterScene_              = false;
            usingEntityId_           = _entity->getID();
            sceneChangerComponentId_ = i;
            return;
        }
    }
}

void SceneTransition::EnterSceneUpdate() {

    Entity* enterSceneEntity = getEntity(usingEntityId_);
    if (enterSceneEntity == nullptr) {
        return;
    }

    currentTransitionTime_ -= Engine::getInstance()->getDeltaTime();

    ///====================================================================
    /// 遷移時間が0以下になった場合、再生終了
    ///====================================================================
    if (currentTransitionTime_ <= 0.0f) {
        currentTransitionTime_ = 0.0f;
        enterScene_            = false;
        exitScene_             = false;

        uint32_t compSize = (uint32_t)getComponentArray<DissolveEffectParam>()->getComponentSize(enterSceneEntity);
        if (compSize <= 0) {
            return;
        }

        for (uint32_t i = 0; i < compSize; ++i) {
            DissolveEffectParam* dissolveEffectParam = getComponent<DissolveEffectParam>(enterSceneEntity, i);
            if (dissolveEffectParam == nullptr) {
                return;
            }
            dissolveEffectParam->Stop();
        }

        return;
    }

    uint32_t compSize = (uint32_t)getComponentArray<DissolveEffectParam>()->getComponentSize(enterSceneEntity);
    if (compSize <= 0) {
        return;
    }

    for (uint32_t i = 0; i < compSize; ++i) {
        DissolveEffectParam* dissolveEffectParam = getComponent<DissolveEffectParam>(enterSceneEntity, i);
        if (dissolveEffectParam == nullptr) {
            return;
        }
        if (!dissolveEffectParam->isActive()) {
            dissolveEffectParam->Play();
        }
        dissolveEffectParam->setThreshold(EaseInQuad(currentTransitionTime_ / maxTransitionTime_));
    }
}

void SceneTransition::ExitSceneUpdate() {
    currentTransitionTime_ += Engine::getInstance()->getDeltaTime();
    if (currentTransitionTime_ >= maxTransitionTime_) {
        currentTransitionTime_ = maxTransitionTime_;
        enterScene_            = false;
        exitScene_             = false;

        // シーン変更を実行
        Entity* sceneChangerEntity = getEntity(usingEntityId_);
        SceneChanger* sceneChanger     = getComponent<SceneChanger>(sceneChangerEntity, sceneChangerComponentId_);
        SceneManager::getInstance()->changeScene(sceneChanger->getNextSceneName());
    }

    Entity* enterSceneEntity = getEntity(usingEntityId_);
    if (enterSceneEntity == nullptr) {
        return;
    }

    uint32_t compSize = (uint32_t)getComponentArray<DissolveEffectParam>()->getComponentSize(enterSceneEntity);
    if (compSize <= 0) {
        return;
    }

    for (uint32_t i = 0; i < compSize; ++i) {
        DissolveEffectParam* dissolveEffectParam = getComponent<DissolveEffectParam>(enterSceneEntity, i);
        if (dissolveEffectParam == nullptr) {
            return;
        }
        if (!dissolveEffectParam->isActive()) {
            dissolveEffectParam->Play();
        }
        dissolveEffectParam->setThreshold(EaseInQuad(currentTransitionTime_ / maxTransitionTime_));
    }
}
