#include "PlayerAnimationStatus.h"

// 外部ライブラリなど
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void PlayerAnimationStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    // 必要であれば初期化処理をここに記述
}

bool PlayerAnimationStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG
    isChange |= CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();
    ImGui::Text("Position");
    isChange |= DragGuiCommand("startPosY", startPosY_, 0.01f);
    isChange |= DragGuiCommand("endPosY", endPosY_, 0.01f);
    isChange |= DragGuiCommand("baseYOffset", baseYOffset_, 0.01f);

    ImGui::Spacing();
    ImGui::Text("Scale");
    isChange |= DragGuiVectorCommand("baseScale", baseScale_, 0.01f);
    isChange |= DragGuiVectorCommand("landingScale", landingScale_, 0.01f);
    isChange |= DragGuiVectorCommand("waitScale", waitScale_, 0.01f);

    ImGui::Spacing();
    ImGui::Text("Launch");
    isChange |= ImGui::SliderAngle("launchRotation", &launchRotationValue_, 0.01f);

    ImGui::Spacing();
    ImGui::Text("Easing - Move");
    isChange |= DragGuiCommand("moveEasing.maxTime", moveEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("moveEasing.backRatio", moveEasing_.backRatio, 0.01f);

    ImGui::Text("Easing - Wait");
    isChange |= DragGuiCommand("waitEasing.maxTime", waitEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("waitEasing.backRatio", waitEasing_.backRatio, 0.01f);

    ImGui::Text("Easing - Landing");
    isChange |= DragGuiCommand("landingEasing_.maxTime", landingEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("landingEasing_.backRatio", landingEasing_.backRatio, 0.01f);

    ImGui::Text("Easing - Launch");
    isChange |= DragGuiCommand("launchEasing.maxTime", launchEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("launchEasing.backRatio", launchEasing_.backRatio, 0.01f);
    isChange |= DragGuiCommand("launchEasing.amplitude", launchEasing_.amplitude, 0.01f);
    isChange |= DragGuiCommand("launchEasing.period", launchEasing_.period, 0.01f);

    ImGui::Text("Easing - LaunchRotate");
    isChange |= DragGuiCommand("launchRotateEasing_.maxTime", launchRotateEasing_.maxTime, 0.01f);

#endif
    return isChange;
}

void PlayerAnimationStatus::MoveAnimaiton(const float& deltaTime) {
    moveEasing_.time += deltaTime;

    /// 　ジャンプイージング
    jumpPosY_ = Back::InCircZero(startPosY_, endPosY_, moveEasing_.time, moveEasing_.maxTime, moveEasing_.backRatio);

    if (moveEasing_.time < moveEasing_.maxTime) {
        return;
    }

    moveEasing_.time = 0.0f;
    jumpPosY_        = startPosY_;
    isLanding_       = true;
}
void PlayerAnimationStatus::LandingAnimation(const float& deltaTime) {
    if (!isLanding_) {
        return;
    }
    landingEasing_.time += deltaTime;

    /// 　ジャンプイージング
    animationScale_ = Back::InCircZero(Vec3f(0.0f, 0.0f, 0.0f), landingScale_, landingEasing_.time, landingEasing_.maxTime, landingEasing_.backRatio);

    if (landingEasing_.time < landingEasing_.maxTime) {
        return;
    }

    landingEasing_.time = 0.0f;
    animationScale_     = Vec3f(0.0f, 0.0f, 0.0f);
    isLanding_          = false;
}

void PlayerAnimationStatus::WaitAnimation(const float& deltaTime, const Vec3f& blockSize) {
    deltaTime;
    animationScale_ = blockSize - waitScale_;
    /* waitEasing_.time += deltaTime;

     /// 　ジャンプイージング
     animationScale_ = Back::InCircZero(Vec3f(0.0f, 0.0f, 0.0f), waitScale_, waitEasing_.time, waitEasing_.maxTime, waitEasing_.backRatio);

     if (waitEasing_.time < waitEasing_.maxTime) {
         return;
     }

     waitEasing_.time = 0.0f;
     animationScale_     = Vec3f(0.0f, 0.0f, 0.0f);*/
}

void PlayerAnimationStatus::LaunchScaleAnimation(const float& deltaTime) {
    launchEasing_.time += deltaTime;

    /// 弾発射アニメーション
    launchScale_ = EaseAmplitudeScale(Vec3f(0.0f, 0.0f, 0.0f), launchEasing_.time, launchEasing_.maxTime, launchEasing_.amplitude, launchEasing_.period);

    if (launchEasing_.time < launchEasing_.maxTime) {
        return;
    }

    launchEasing_.time = launchEasing_.maxTime;
    launchScale_        = Vec3f(0.0f, 0.0f, 0.0f);
}

void PlayerAnimationStatus::LaunchRotateAnimation(const float& deltaTime) {
    launchRotateEasing_.time += deltaTime;

    /// 弾発射アニメーション
    launchRotate_ = EaseOutQuad(0.0f, launchRotationValue_, launchRotateEasing_.time, launchRotateEasing_.maxTime);

    if (launchRotateEasing_.time < launchRotateEasing_.maxTime) {
        return;
    }

    launchRotateEasing_.time = launchRotateEasing_.maxTime;
    launchRotate_            = 0.0f;
  
}

void PlayerAnimationStatus::ChangeMotionWait() {
    if (launchRotateEasing_.time < launchRotateEasing_.maxTime) {
        return;
    }
    if (launchEasing_.time < launchEasing_.maxTime) {
        return;
    }

      motionStep_ = MotionStep::WAIT;
  }

void PlayerAnimationStatus::ChangeMotion(const MotionStep& step) {

   
    if (motionStep_ == step) {
        return;
    }

    switch (step) {
    case PlayerAnimationStatus::MotionStep::WAIT:
        if (!isLanding_ || motionStep_==MotionStep::LAUNCH) {
            return;
        }
        break;
    case PlayerAnimationStatus::MotionStep::MOVE:
        if ( motionStep_ == MotionStep::LAUNCH) {
            return;
        }
        break;
    case PlayerAnimationStatus::MotionStep::LAUNCH:
      
        break;
    default:
        break;
    }

    Reset();

    motionStep_ = step;
}

void PlayerAnimationStatus::Reset() {
    waitEasing_.time         = 0.0f;
    moveEasing_.time         = 0.0f;
    landingEasing_.time      = 0.0f;
    launchEasing_.time       = 0.0f;
    launchRotateEasing_.time = 0.0f;
}

void to_json(nlohmann::json& j, const PlayerAnimationStatus& m) {
    j["isAlive"]        = m.isAlive_;
    j["startPosY"]      = m.startPosY_;
    j["endPosY"]        = m.endPosY_;
    j["baseYOffset"]    = m.baseYOffset_;
    j["landingScale"]   = m.landingScale_;
    j["waitScale"]      = m.waitScale_;
    j["launchRotation"] = m.launchRotationValue_;

    // moveEasing

    j["moveEasing.maxTime"]   = m.moveEasing_.maxTime;
    j["moveEasing.backRatio"] = m.moveEasing_.backRatio;
    j["moveEasing.amplitude"] = m.moveEasing_.amplitude;
    j["moveEasing.period"]    = m.moveEasing_.period;

    // waitEasing

    j["waitEasing.maxTime"]   = m.waitEasing_.maxTime;
    j["waitEasing.backRatio"] = m.waitEasing_.backRatio;
    j["waitEasing.amplitude"] = m.waitEasing_.amplitude;
    j["waitEasing.period"]    = m.waitEasing_.period;

    // launchEasing

    j["launchEasing.maxTime"]   = m.launchEasing_.maxTime;
    j["launchEasing.backRatio"] = m.launchEasing_.backRatio;
    j["launchEasing.amplitude"] = m.launchEasing_.amplitude;
    j["launchEasing.period"]    = m.launchEasing_.period;

    // launchRotate

    j["launchRotateEasing.maxTime"]   = m.launchRotateEasing_.maxTime;
    j["launchRotateEasing.backRatio"] = m.launchRotateEasing_.backRatio;
    j["launchRotateEasing.amplitude"] = m.launchRotateEasing_.amplitude;
    j["launchRotateEasing.period"]    = m.launchRotateEasing_.period;

    // landing
    j["landingEasing.maxTime"]   = m.landingEasing_.maxTime;
    j["landingEasing.backRatio"] = m.landingEasing_.backRatio;
    j["landingEasing.amplitude"] = m.landingEasing_.amplitude;
    j["landingEasing.period"]    = m.landingEasing_.period;

    j["baseScale_"] = m.baseScale_;
}

void from_json(const nlohmann::json& j, PlayerAnimationStatus& m) {
    j.at("isAlive").get_to(m.isAlive_);
    j.at("startPosY").get_to(m.startPosY_);
    j.at("endPosY").get_to(m.endPosY_);
    j.at("baseYOffset").get_to(m.baseYOffset_);
    j.at("landingScale").get_to(m.landingScale_);
    j.at("waitScale").get_to(m.waitScale_);
    j.at("launchRotation").get_to(m.launchRotationValue_);

    // moveEasing

    j.at("moveEasing.maxTime").get_to(m.moveEasing_.maxTime);
    j.at("moveEasing.backRatio").get_to(m.moveEasing_.backRatio);
    j.at("moveEasing.amplitude").get_to(m.moveEasing_.amplitude);
    j.at("moveEasing.period").get_to(m.moveEasing_.period);

    // waitEasing

    j.at("waitEasing.maxTime").get_to(m.waitEasing_.maxTime);
    j.at("waitEasing.backRatio").get_to(m.waitEasing_.backRatio);
    j.at("waitEasing.amplitude").get_to(m.waitEasing_.amplitude);
    j.at("waitEasing.period").get_to(m.waitEasing_.period);

    // launchEasing
    j.at("launchEasing.maxTime").get_to(m.launchEasing_.maxTime);
    j.at("launchEasing.backRatio").get_to(m.launchEasing_.backRatio);
    j.at("launchEasing.amplitude").get_to(m.launchEasing_.amplitude);
    j.at("launchEasing.period").get_to(m.launchEasing_.period);

    // launchRotateEasing
    j.at("launchRotateEasing.maxTime").get_to(m.launchRotateEasing_.maxTime);
    j.at("launchRotateEasing.backRatio").get_to(m.launchRotateEasing_.backRatio);
    j.at("launchRotateEasing.amplitude").get_to(m.launchRotateEasing_.amplitude);
    j.at("launchRotateEasing.period").get_to(m.launchRotateEasing_.period);

    // launchRotateEasing
    j.at("landingEasing.maxTime").get_to(m.landingEasing_.maxTime);
    j.at("landingEasing.backRatio").get_to(m.landingEasing_.backRatio);
    j.at("landingEasing.amplitude").get_to(m.landingEasing_.amplitude);
    j.at("landingEasing.period").get_to(m.landingEasing_.period);
    if (auto it = j.find("baseScale_"); it != j.end()) {
        j.at("baseScale_").get_to(m.baseScale_);
    }
}

void PlayerAnimationStatus::Finalize() {
    // 必要ならリソース解放などを記述
}
