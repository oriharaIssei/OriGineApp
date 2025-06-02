#include "BlockStatus.h"
#include "component/transform/Transform.h"
#include "KetaShake.h"

/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"
#include <string>

void BlockStatus::Initialize([[maybe_unused]] GameEntity* _entity) {

    currentHP_ = 1;
}

bool BlockStatus::Edit() {
    bool isChange = false;
#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

#endif // _DEBUG

    return isChange;
}

void BlockStatus::Finalize() {}

void BlockStatus::TakeDamageForBomb() {
    currentHP_--;
}

void BlockStatus::TakeDamageForBigBomb() {
    isNotScrapSpawn_ = true;
    currentHP_       = 0;
}

void BlockStatus::TakeDamageForFloor() {
    currentHP_ = 0;
}

void BlockStatus::MoveUpdate(const float& time, Transform* transform, const float& moveValue) {
    transform;

    if (!isMove_) {
        return;
    }

    // 移動位置
    float movepos = preMovePos_[X] - (moveValue * 2.0f);
    moveEase_.time += time;

    /// スケーリングイージング
    basePos_[X] = (EaseInBounce(preMovePos_[X], movepos, moveEase_.time, moveEase_.maxTime));

    if (moveEase_.time >= moveEase_.maxTime) {
        // save time
        moveEase_.time = moveEase_.maxTime;

        // save pos
        basePos_[X]     = movepos;
        preMovePos_[X] = basePos_[X];

        // flag increment row
        isMove_ = false;
        rowNum_++;
    }
}

void BlockStatus::BreakTimerDecrement(const float& deltaTime) {
    breakOffsetTime_ -= deltaTime;
}

void BlockStatus::TimeInit() {
    moveEase_.time = 0.0f;
}

void to_json(nlohmann::json& _json, const BlockStatus& _block) {
    _json["isAlive"] = _block.isAlive_;
}

void from_json(const nlohmann::json& _json, BlockStatus& _block) {
    _json.at("isAlive").get_to(_block.isAlive_);
}

void BlockStatus::SetBlockManagerParm(BlockManager* parm) {

    breakApearEasing_ = parm->GetBreakApearEasing();
    startZPos_        = parm->GetStartPositionZ();
    EndZPos_          = parm->GetEndZPos();
    breakBackEasing_  = parm->GetBreakBackEasing();
    saveZPos_         = startZPos_;
    zposition_        = saveZPos_;
}

void BlockStatus::SavingZPosition(/*const float& pos*/) {
    saveZPos_ = zposition_;
}

void BlockStatus::ZApearEasing(const float& deltaTime) {
    breakApearEasing_.time += deltaTime;

    /// スケーリングイージング
    zposition_ = (EaseInCirc(saveZPos_, EndZPos_, breakApearEasing_.time, breakApearEasing_.maxTime));

    if (breakApearEasing_.time < breakApearEasing_.maxTime) {
        return;
    }
    // save time
    breakApearEasing_.time = breakApearEasing_.maxTime;
    zposition_             = EndZPos_;
}

void BlockStatus::ZBackEasing(const float& deltaTime) {
    breakBackEasing_.time += deltaTime;

    /// スケーリングイージング
    zposition_ = (EaseInCirc(saveZPos_, startZPos_, breakBackEasing_.time, breakBackEasing_.maxTime));

    if (breakBackEasing_.time < breakBackEasing_.maxTime) {
        return;
    }
    // save time
    breakBackEasing_.time = breakBackEasing_.maxTime;
    zposition_            = startZPos_;
    apearAnimationStep_   = ApearAnimationStep::END;
}

void BlockStatus::ChangeStep(const ApearAnimationStep& step) {
    // timer reset
    TimerReset();
    if (apearAnimationStep_ == step) {
        return;
    }
    apearAnimationStep_ = step;
}

void BlockStatus::SinisterShake() {
    if (!isSinister_) {
        sinisterOffsetPos_ = Vec3f(0.0f, 0.0f, 0.0f);
        return;
    }
    sinisterOffsetPos_ = Shake<Vec3f>(0.7f, 0.8f);
}

void BlockStatus::TimerReset() {
    saveZPos_              = zposition_;
    breakApearEasing_.time = 0.0f;
    breakBackEasing_.time  = 0.0f;
}
