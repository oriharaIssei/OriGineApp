#include "TutorialMenuParentStatus.h"

// 外部ライブラリなど
#include "imgui/imgui.h"

void TutorialMenuParentStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    // 必要であれば初期化処理をここに記述
}

bool TutorialMenuParentStatus::Edit() {
    bool isChange = false;

    isChange |= ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    ImGui::Text("pos");
    isChange |= ImGui::DragFloat2("initPos", initPos_.v, 0.01f);
    isChange |= ImGui::DragFloat2("scrollValue", scrollValue_.v, 0.01f);
    isChange |= ImGui::DragFloat2("endPos", endPos_.v, 0.01f);

    ImGui::Text("easing");
    isChange |= ImGui::DragFloat("moveEasing.maxTime", &moveEasing_.maxTime, 0.01f);
    isChange |= ImGui::DragFloat("apperUVAnimation.maxTime", &apperUVEasing_.maxTime, 0.01f);

    ImGui::Text("etc");
    isChange |= ImGui::InputInt("maxPauge", &maxPauge_);

    return isChange;
}

void to_json(nlohmann::json& j, const TutorialMenuParentStatus& m) {
    j["isAlive"]               = m.isAlive_;
    j["moveEasing.maxTime"]    = m.moveEasing_.maxTime;
    j["apperUVEasing.maxTime"] = m.apperUVEasing_.maxTime;
    j["initPos"]               = m.initPos_;
    j["endPos"]                = m.endPos_;
    j["scrollValue"]           = m.scrollValue_;
    j["maxPauge"]              = m.maxPauge_;
}

void from_json(const nlohmann::json& j, TutorialMenuParentStatus& m) {
    j.at("isAlive").get_to(m.isAlive_);
    j.at("moveEasing.maxTime").get_to(m.moveEasing_.maxTime);
    j.at("apperUVEasing.maxTime").get_to(m.apperUVEasing_.maxTime);
    j.at("initPos").get_to(m.initPos_);
    j.at("endPos").get_to(m.endPos_);
    j.at("scrollValue").get_to(m.scrollValue_);
    j.at("maxPauge").get_to(m.maxPauge_);
}


void TutorialMenuParentStatus::Finalize() {
    // 必要ならリソース解放などを記述
}

void TutorialMenuParentStatus::MoveAnimation(const float& time) {

    switch (scrollStep_) {
    case ScrollStep::PAUGEUP:
        if (currentPauge_ >= maxPauge_) {
            scrollStep_ = ScrollStep::PAUGESELECTION;
            return;
        }
        savePos_ = startPos_;
        endPos_  = startPos_ + scrollValue_;
        break;
    case ScrollStep::PAUGEDOWN:
        if (currentPauge_ <= 0) {
            scrollStep_ = ScrollStep::PAUGESELECTION;
            return;
        }
        savePos_ = startPos_;
        endPos_  = startPos_ - scrollValue_;
        break;
    }

    moveEasing_.time += time;

    position_ = EaseOutBack(savePos_, endPos_, moveEasing_.time, moveEasing_.maxTime);

    if (moveEasing_.time < moveEasing_.maxTime) {
        return;
    }
    moveEasing_.time = moveEasing_.maxTime;
    position_        = endPos_;
    startPos_        = position_;

     switch (scrollStep_) {
    case ScrollStep::PAUGEUP:
        currentPauge_++;
        break;
    case ScrollStep::PAUGEDOWN:
        currentPauge_--;
        break;
    }


    scrollStep_      = ScrollStep::PAUGESELECTION;

   
}

void TutorialMenuParentStatus::FirstMoveAnimation(const float& time) {
    apperUVEasing_.time += time;

    scaleX_ = EaseInCubic(0.0f, 1.0f, apperUVEasing_.time, apperUVEasing_.maxTime);

    if (apperUVEasing_.time < apperUVEasing_.maxTime) {
        return;
    }
    scaleX_             = 1.0f;
    apperUVEasing_.time =0.0f;
    scrollStep_         = ScrollStep::PAUGESELECTION;
}

void TutorialMenuParentStatus::BackSizeAnimation(const float& time) {
    apperUVEasing_.time += time;

    scaleX_ = EaseInCubic(1.0f,0.0f, apperUVEasing_.time, apperUVEasing_.maxTime);

    if (apperUVEasing_.time < apperUVEasing_.maxTime) {
        return;
    }
    scaleX_             = 0.0f;
    apperUVEasing_.time = 0.0f;
    scrollStep_         = ScrollStep::END;
}

void TutorialMenuParentStatus::Reset() {
    moveEasing_.time    = 0.0f;
    apperUVEasing_.time = 0.0f;
    scaleX_             = 0.0f;
    currentPauge_       = 0;
    position_           = initPos_;
    startPos_           = position_;
}

void TutorialMenuParentStatus::ScrollTimeReset() {
    moveEasing_.time = 0.0f;
}
