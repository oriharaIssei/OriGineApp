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

void TutorialMenuParentStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("moveEasing.maxTime", moveEasing_.maxTime);
    _writer.Write("apperUVEasing.maxTime", apperUVEasing_.maxTime);
    _writer.Write<2, float>("initPos", initPos_);
    _writer.Write<2, float>("endPos", endPos_);
    _writer.Write<2, float>("scrollValue", scrollValue_);
    _writer.Write("maxPauge", maxPauge_);
}

void TutorialMenuParentStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("moveEasing.maxTime", moveEasing_.maxTime);
    _reader.Read("apperUVEasing.maxTime", apperUVEasing_.maxTime);
    _reader.Read<2, float>("initPos", initPos_);
    _reader.Read<2, float>("endPos", endPos_);
    _reader.Read<2, float>("scrollValue", scrollValue_);
    _reader.Read("maxPauge", maxPauge_);
}

void TutorialMenuParentStatus::Finalize() {
    // 必要ならリソース解放などを記述
}

void TutorialMenuParentStatus::MoveAnimation(const float& time) {

    switch (scrollStep_) {
    case ScrollStep::PAUGEUP:
        if (currentPauge_ >= maxPauge_) {
            return;
        }
        savePos_ = startPos_;
        endPos_  = startPos_ + scrollValue_;
        break;
    case ScrollStep::PAUGEDOWN:
        if (currentPauge_ <= 0) {
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
    scrollStep_      = ScrollStep::PAUGESELECTION;
}

void TutorialMenuParentStatus::FirstMoveAnimation(const float& time) {
    apperUVEasing_.time += time;

    uvScale_ = EaseOutBack(0.0f, 1.0f, apperUVEasing_.time, apperUVEasing_.maxTime);

    if (apperUVEasing_.time < apperUVEasing_.maxTime) {
        return;
    }
    uvScale_            = 1.0f;
    apperUVEasing_.time = apperUVEasing_.maxTime;
    scrollStep_         = ScrollStep::PAUGESELECTION;
}
void TutorialMenuParentStatus::BackUVAnimation(const float& time) {
    apperUVEasing_.time -= time;

    uvScale_ = EaseOutBack(0.0f, 1.0f, apperUVEasing_.time, apperUVEasing_.maxTime);

    if (apperUVEasing_.time > 0.0f) {
        return;
    }
    uvScale_            = 0.0f;
    apperUVEasing_.time = 0.0f;
    scrollStep_         = ScrollStep::END;
}

void TutorialMenuParentStatus::Reset() {
    moveEasing_.time    = 0.0f;
    apperUVEasing_.time = 0.0f;
    uvScale_            = 0.0f;
    position_           = initPos_;
    startPos_           = position_;
}
