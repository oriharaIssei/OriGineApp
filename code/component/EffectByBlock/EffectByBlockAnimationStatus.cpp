#include "EffectByBlockAnimationStatus.h"
/// engine
#define ENGINE_INCLUDE
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void to_json(nlohmann::json& _json, const EffectByBlockAnimationStatus& _component) {
    _json["isAlive"] = _component.isAlive_;
}
void from_json(const nlohmann::json& _json, EffectByBlockAnimationStatus& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
}

void EffectByBlockAnimationStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool EffectByBlockAnimationStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

#endif

    return isChange;
}

void EffectByBlockAnimationStatus::Finalize() {}

void EffectByBlockAnimationStatus::ApearEasing(const float& deltaTime) {
    ApearEasing_.time += deltaTime;

    baseScale_ = EaseOutBounce(Vec3f(0.0f, 0.0f, 0.0f), maxScale_, ApearEasing_.time, ApearEasing_.maxTime);

    // end
    if (ApearEasing_.time < ApearEasing_.maxTime) {
        return;
    }

    baseScale_        = maxScale_;
    ApearEasing_.time = ApearEasing_.maxTime;
    animationStep_    = AnimationStep::WAIT;
}

void EffectByBlockAnimationStatus::FadeOutEasing(const float& deltaTime) {
    rotate_ += rotateSpeed_ * deltaTime;
    FadeOutEasing_.time += deltaTime;

    basePosX_ = EaseInCirc(savePosX_, savePosX_ + fadeOutValueX_, FadeOutEasing_.time, FadeOutEasing_.maxTime);

    // end
    if (FadeOutEasing_.time < FadeOutEasing_.maxTime) {
        return;
    }

    basePosX_           = savePosX_ + fadeOutValueX_;
    FadeOutEasing_.time = FadeOutEasing_.maxTime;
 /*   animationStep_      = AnimationStep::END;*/
}

void EffectByBlockAnimationStatus::FadeOutWaitUpdate(const float& deltaTime,const float&savePos) {
    currentTime_ += deltaTime;

    if (currentTime_ < fadeOutTime_) {
        return;
    }

    savePosX_ = savePos;
    animationStep_ = AnimationStep::FADEOUT;
}

void EffectByBlockAnimationStatus::Reset() {
    ApearEasing_.time = 0.0f;
    FadeOutEasing_.time = 0.0f;
    rotate_      = 0.0f;
    basePosX_    = 0.0f;
    currentTime_ = 0.0f;
    baseScale_   = Vec3f(0.0f, 0.0f, 0.0f);

}
