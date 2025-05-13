#include "SceneTransition.h"
#include "component/SceneChanger/SceneChangerStatus.h"
#include "imgui/imgui.h"

bool SceneTransition::isTitleTransitionOut_ = false;

void to_json(nlohmann::json& j, const SceneTransition& s) {
    j["TransitionMode"]   = static_cast<int32_t>(s.transitionMode_);
    j["CurrentScene"]     = static_cast<int32_t>(s.currentScene_);
    j["SceneEaseMaxTime"] = s.sceneEase_.maxTime;
    j["waitTime"]         = s.waitTime_;
    j["startPositionX_"]  = s.startPositionX_;
    j["endPositionX_"]    = s.endPositionX_;
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

    if (auto it = j.find("startPositionX_"); it != j.end()) {
        j.at("startPositionX_").get_to(s.startPositionX_);
    }

    if (auto it = j.find("endPositionX_"); it != j.end()) {
        j.at("endPositionX_").get_to(s.endPositionX_);
    }
}

void SceneTransition::Initialize([[maybe_unused]] GameEntity* _entity) {
    // 初期化時に必要であれば設定
    sceneEase_.time = 0.0f;
    positionX_      = 0.0f;
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

    isChange |= DragGuiCommand("Scene Ease MaxTime", &sceneEase_.maxTime, 0.01f);
    isChange |= DragGuiCommand("waitTime", &waitTime_, 0.01f);

    isChange |= DragGuiCommand("startPositionX", &startPositionX_, 0.01f);
    isChange |= DragGuiCommand("endPositionX", &endPositionX_, 0.01f);

    return isChange;
}

void SceneTransition::Finalize() {
    // リソースの後始末など必要なら
}

void SceneTransition::TransitionInit() {
    positionX_       = -1280.0f;
    sceneEase_.time  = 0.0f;
    currentWaitTime_ = 0.0f;
    scale_           = Vec2f(0.0f, 0.0f);
}

void SceneTransition::UpdateTransition(const float& deltaTime) {

    sceneEase_.time += deltaTime;

   /* switch (transitionMode_) {
    case TransitionMode::FadeIN:
        startPositionX_ = -1280;
        endPositionX_   = 0;
        break;
    case TransitionMode::FadeOUT:
        startPositionX_ = 0;
        endPositionX_   = -1280;
        break;
    default:
        break;
    }*/

    positionX_ = EaseInCubic(startPositionX_, endPositionX_, sceneEase_.time, sceneEase_.maxTime);

    if (sceneEase_.time < sceneEase_.maxTime) {
        return;
    }

    sceneEase_.time = sceneEase_.maxTime;
    positionX_      = endPositionX_;
    transitionStep_ = TransitonStep::WAIT;
}

void SceneTransition::WaitUpdate(const float& deltaTime) {

    currentWaitTime_ += deltaTime;
    if (currentWaitTime_ < waitTime_) {
        return;
    }

    transitionStep_ = TransitonStep::END;
}

void SceneTransition::GoToNextScene(SceneChangerStatus* SceneChanger) {
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

    sceneEase_.time  = 0.0f;
    currentWaitTime_ = 0.0f;
    positionX_       = startPositionX_;
    scale_           = Vec2f(1.0f, 1.0f);
}

bool SceneTransition::IsAnimationEnd() {
    return (transitionStep_ == TransitonStep::END);
}

bool SceneTransition::IsAbleAnimationStart() {
    return !(GetTransitionnMode() == TransitionMode::FadeIN && !GetIsTransitionIn());
}
bool SceneTransition::IsAbleTitleOutAnimationStart() {
    // タイトルのフェードアウト用
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
