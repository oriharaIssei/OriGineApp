#include "BlockStatus.h"
#include "component/transform/Transform.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"
#include <string>

void BlockStatus::Initialize([[maybe_unused]] GameEntity* _entity) {

    currentHP_ = 1;
    /* collisionSize_ = 5.0f;*/
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

    if (!isMove_) {
        return;
    }

    // 移動位置
    float movepos = preMovePos_[X] - moveValue;
    moveEase_.time += time;

    /// スケーリングイージング
    transform->translate[X] = (EaseInCirc(preMovePos_[X], movepos, moveEase_.time, moveEase_.maxTime));

    if (moveEase_.time >= moveEase_.maxTime) {
        // save time
        moveEase_.time = moveEase_.maxTime;

        // save pos
        transform->translate[X] = movepos;
        preMovePos_[X]          = transform->translate[X];

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
