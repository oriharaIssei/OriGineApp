#include "SceneTransition.h"
#include "component/GameEnd/GameEnd.h"
#include "imgui/imgui.h"

bool SceneTransition::isTitleTransitionOut_ = false;

void to_json(nlohmann::json& j, const SceneTransition& s) {
    j["TransitionMode"]   = static_cast<int32_t>(s.transitionMode_);
    j["CurrentScene"]     = static_cast<int32_t>(s.currentScene_);
    j["SceneEaseMaxTime"] = s.sceneEase_.maxTime;
    j["waitTime"]         = s.waitTime_;
}
void from_json(const nlohmann::json& j, SceneTransition& s) {
    int32_t mode    = 0;
    int32_t current = 0;

    j.at("TransitionMode").get_to(mode);
    j.at("CurrentScene").get_to(current);

    s.transitionMode_ = static_cast<TransitionMode>(mode);
    s.currentScene_   = static_cast<TransitionScene>(current);

    j.at("SceneEaseMaxTime").get_to(s.sceneEase_.maxTime);
    j.at("waitTime").get_to(s.waitTime_);
}

void SceneTransition::Initialize([[maybe_unused]] GameEntity* _entity) {
    // 初期化時に必要であれば設定
    sceneEase_.time = 0.0f;
    transitionPosX_ = 0.0f;
}

bool SceneTransition::Edit() {
    bool isChange = false;

    const char* modeItems[] = {"IN", "OUT"};
    int modeIndex           = static_cast<int>(transitionMode_);
    if (ImGui::Combo("Transition Mode", &modeIndex, modeItems, IM_ARRAYSIZE(modeItems))) {
        transitionMode_ = static_cast<TransitionMode>(modeIndex);
        isChange        = true;
    }

    const char* sceneItems[] = {"TITLE", "GAME", "RESLUT"};
    int currentSceneIndex    = static_cast<int>(currentScene_);
    if (ImGui::Combo("Current Scene", &currentSceneIndex, sceneItems, IM_ARRAYSIZE(sceneItems))) {
        currentScene_ = static_cast<TransitionScene>(currentSceneIndex);
        isChange      = true;
    }

    isChange |= ImGui::DragFloat("Scene Ease MaxTime", &sceneEase_.maxTime, 0.01f);
    isChange |= ImGui::DragFloat("waitTime", &waitTime_, 0.01f);

    return isChange;
}

void SceneTransition::Finalize() {
    // リソースの後始末など必要なら
}

void SceneTransition::TransitionInit() {
    transitionPosX_  = -1280.0f;
    sceneEase_.time  = 0.0f;
    currentWaitTime_ = 0.0f;
    scale_           = Vec2f(0.0f, 0.0f);
}

void SceneTransition::UpdateTransition(const float& deltaTime) {

    sceneEase_.time += deltaTime;

    switch (transitionMode_) {
    case TransitionMode::FadeIN:
        startPos_ = -1280;
        endPos_   = 0;
        break;
    case TransitionMode::FadeOUT:
        startPos_ = 0;
        endPos_   = -1280;
        break;
    default:
        break;
    }

    transitionPosX_ = EaseInCubic(startPos_, endPos_, sceneEase_.time, sceneEase_.maxTime);

    if (sceneEase_.time < sceneEase_.maxTime) {
        return;
    }

    sceneEase_.time = sceneEase_.maxTime;
    transitionPosX_ = endPos_;
    transitionStep_ = TransitonStep::WAIT;
}

void SceneTransition::WaitUpdate(const float& deltaTime) {

    currentWaitTime_ += deltaTime;
    if (currentWaitTime_ < waitTime_) {
        return;
    }

    transitionStep_ = TransitonStep::END;
}

void SceneTransition::GoToNextScene(GameEnd* SceneChanger) {
    if (transitionMode_ == TransitionMode::FadeOUT) {
        return;
    }

    // シーンによって動きを帰る
    switch (currentScene_) {
    case TransitionScene::TITLE:
        isTitleTransitionOut_ = false;
        SceneChanger->SetIsGoToGame(true);
        break;
    case TransitionScene::GAME:
        if (isGoTitleFromMenu_) {
            SceneChanger->SetIsBackTitle(true);
        } else {
            SceneChanger->SetIsGoToResult(true);
        }
        break;
    case TransitionScene::RESLUT:
        if (isRetry_) {
            SceneChanger->SetIsGoToGame(true);
        } else {

            SceneChanger->SetIsBackTitle(true);
        }
        break;
    default:
        break;
    }
}

void SceneTransition::Reset() {
    transitionPosX_  = 0.0f;
    sceneEase_.time  = 0.0f;
    currentWaitTime_ = 0.0f;
    scale_           = Vec2f(1.0f, 1.0f);
}

bool SceneTransition::IsAnimationEnd() {
    return (transitionStep_ == TransitonStep::END);
}

bool SceneTransition::IsAbleAnimationStart() {
    return !(GetTransitionnMode() == TransitionMode::FadeIN && !GetIsTransitionIn());
}
bool SceneTransition::IsAbleTitleOutAnimationStart() {
    if (currentScene_ == TransitionScene::TITLE && transitionMode_ == TransitionMode::FadeOUT) {

        if (!isTitleTransitionOut_) {
            return false;
        } else {
            return true;
        }
    } else {
        return true;
    }
}
