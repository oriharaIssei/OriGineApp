#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <Entity.h>
#include <Vector2.h>


enum class TransitionMode {
    FadeIN,
    FadeOUT,
};

enum class TransitionScene {
    TITLE,
    GAME,
    RESLUT,
};

enum class TransitonStep {
    NONE,
    MOVE,
    WAIT,
    END,
};
class GameEnd;
class SceneTransition
    : public IComponent {

    friend void to_json(nlohmann::json& j, const SceneTransition& s);
    friend void from_json(const nlohmann::json& j, SceneTransition& s);

public:
    SceneTransition() {}
    virtual ~SceneTransition() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

    void TransitionInit();
    void UpdateTransition(const float& deltaTime);
    void WaitUpdate(const float& deltaTime);
    void Reset();

    bool IsAnimationEnd();

    bool IsAbleAnimationStart();
    bool IsAbleTitleOutAnimationStart();

private:
    TransitionMode transitionMode_;
    TransitionScene currentScene_  = TransitionScene::TITLE;
    TransitonStep transitionStep_  = TransitonStep::NONE;
   
    bool isTransitionIn_=false;

    Easing sceneEase_;
    float transitionPosX_;

    float endPos_;
    float startPos_;

    float waitTime_;
    float currentWaitTime_;

    bool isGoTitleFromMenu_ = false;
    bool isRetry_           = false;
    static bool isTitleTransitionOut_;

    Vec2f scale_ = {0.0f,0.0f};

    public:
 
    void SetIsTransitionIn(const bool& is) { isTransitionIn_ = is; }
    void SetTransitionStep(const TransitonStep& step) { transitionStep_ = step; }
    void GoToNextScene(GameEnd*gameend);
    void SetIsGoTitleFromMenu(const bool& is) { isGoTitleFromMenu_ = is; }
    void SetIsRetry(const bool& is) { isRetry_ = is; }
    void SettIsTitleTransitionOut(const bool& is) { isTitleTransitionOut_ = is; }

    // カメラなどの取得に使用
    bool GetIsTransitionIn() const { return isTransitionIn_; }
    float TransitionPosX() const { return transitionPosX_; }
    TransitionScene GetCurrentScene() const { return currentScene_; }
    TransitonStep GetTransitionStep() const { return transitionStep_; }
    TransitionMode GetTransitionnMode() const { return transitionMode_; }
    Vec2f GetScale() const { return scale_; }
    bool GetIsRetry() const { return isRetry_; }
    bool GetIsTitleTransitionOut() const { return isTitleTransitionOut_; }
};
