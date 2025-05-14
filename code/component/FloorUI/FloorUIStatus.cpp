#include "FloorUIStatus.h"

// 外部ライブラリなど
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void FloorUIStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    // 必要であれば初期化処理をここに記述
}

bool FloorUIStatus::Edit() {
    bool isChange = false;

    isChange |= CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    return isChange;
}

void to_json(nlohmann::json& j, const FloorUIStatus& m) {
    j["isAlive"] = m.isAlive_;
}

void from_json(const nlohmann::json& j, FloorUIStatus& m) {
    j.at("isAlive").get_to(m.isAlive_);
}

void FloorUIStatus::Finalize() {
    // 必要ならリソース解放などを記述
}

void FloorUIStatus::moveEasing(const float& time) {
    moveEasing_.time += time;

    position_ = Back::InCircZero(startPos_, endPos_, moveEasing_.time, moveEasing_.maxTime, moveEasing_.backRatio);

    if (moveEasing_.time < moveEasing_.maxTime) {
        return;
    }

    position_        = startPos_;
    moveEasing_.time = 0.0f;
}

void FloorUIStatus::OpenEasing(const float& time) {
    openEasing_.time += time;

    baseScale_ = EaseInCirc(Vec3f(0.0f, 0.0f, 0.0f), maxScale_, openEasing_.time, openEasing_.maxTime);

    if (openEasing_.time < openEasing_.maxTime) {
        return;
    }

    baseScale_       = maxScale_;
    openEasing_.time = 0.0f;
    floorUIStep_     = FloorUIStep::MOVING;
}
void FloorUIStatus::CloseEasing(const float& time) {
    closeEasing_.time += time;

    baseScale_ = EaseInCirc(saveScale_, Vec3f(0.0f, 0.0f, 0.0f), closeEasing_.time, closeEasing_.maxTime);

    if (closeEasing_.time < closeEasing_.maxTime) {
        return;
    }

    baseScale_        = Vec3f(0.0f, 0.0f, 0.0f);
    closeEasing_.time = 0.0f;
    floorUIStep_      = FloorUIStep::END;
}

void FloorUIStatus::Reset() {
    moveEasing_.time  = 0.0f;
    openEasing_.time  = 0.0f;
    closeEasing_.time = 0.0f;
    baseScale_        = Vec3f(0.0f, 0.0f, 0.0f);
    saveScale_        = Vec3f(0.0f, 0.0f, 0.0f);
}

void FloorUIStatus::SavingScale() {
    saveScale_ = baseScale_;
}

void FloorUIStatus::ChangeAnimationEndForFlag() {

    if (*isAnimation_) {
        return;
    }
    floorUIStep_ = FloorUIStep::CLOSE;
}
