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

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    return isChange;
}

void BlockStatus::Finalize() {}

void BlockStatus::SetColum(const int32_t& colum) {

    columNum_ = colum;
}

void BlockStatus::TakeDamageForBomb() {
    currentHP_--;
}

void BlockStatus::TakeDamageForBigBomb() {
    isNotScrapSpawn_ = true;
    currentHP_=0;
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
        moveEase_.time          = moveEase_.maxTime;
        transform->translate[X] = movepos;
        preMovePos_[X]          = transform->translate[X];
        isMove_                 = false;
    }
}

void BlockStatus::TimeInit() {
    moveEase_.time = 0.0f;
}

void to_json(nlohmann::json& _json, const BlockStatus& _block) {
    _json["isAlive"] = _block.isAlive_;
    /*
    _json["columNum"]       = _block.columNum_;
     _json["currentHP"]      = _block.currentHP_;
     _json["isDestroy"]      = _block.isDestroy_;
     _json["isbreak"]        = _block.isbreak_;
     _json["ratio"]          = _block.ratio_;
     _json["baseScoreValue"] = _block.baseScoreValue_;
     _json["blockType"]      = static_cast<int32_t>(_block.blockType_);
     */
}

void from_json(const nlohmann::json& _json, BlockStatus& _block) {
    _json.at("isAlive").get_to(_block.isAlive_);
    /*
   _json.at("columNum").get_to(_block.columNum_);
    _json.at("currentHP").get_to(_block.currentHP_);
    _json.at("isDestroy").get_to(_block.isDestroy_);
    _json.at("isbreak").get_to(_block.isbreak_);
    _json.at("ratio").get_to(_block.ratio_);
    _json.at("baseScoreValue").get_to(_block.baseScoreValue_);
    _json.at("blockType").get_to(static_cast<int32_t>(_block.blockType_));
    */
}
