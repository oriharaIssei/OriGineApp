#include "MenuStatus.h"

// 外部ライブラリなど
#include "imgui/imgui.h"
#include "input/Input.h"

void MenuStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    // 必要であれば初期化処理をここに記述
}

bool MenuStatus::Edit() {
    bool isChange = false;

    isChange |= ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    ImGui::Text("pos");
    isChange |= ImGui::DragFloat2("position", position_.v, 0.01f);
    for (int32_t i = 0; i < arrowPositions_.size(); ++i) {
        isChange |= ImGui::DragFloat2(("position" + std::to_string(i)).c_str(), arrowPositions_[i].v, 0.01f);
    }

    ImGui::Text("easing");
    isChange |= ImGui::DragFloat("moveEasing.maxTime", &moveEasing_.maxTime, 0.01f);
    isChange |= ImGui::DragFloat("apperUVAnimation.maxTime", &apperUVEasing_.maxTime, 0.01f);

    ImGui::Text("etc");
    isChange |= ImGui::InputInt("maxPauge", &maxCategoryNum_);

    return isChange;
}

void MenuStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("moveEasing.maxTime", moveEasing_.maxTime);
    _writer.Write("apperUVEasing.maxTime", apperUVEasing_.maxTime);
    _writer.Write<2, float>("position", position_);
    _writer.Write("maxPauge", maxCategoryNum_);
    for (int32_t i = 0; i < arrowPositions_.size(); ++i) {
        _writer.Write<2, float>(("positions" + std::to_string(i)).c_str(), arrowPositions_[i]);
    }
}

void MenuStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("moveEasing.maxTime", moveEasing_.maxTime);
    _reader.Read("apperUVEasing.maxTime", apperUVEasing_.maxTime);
    _reader.Read<2, float>("position", position_);
    _reader.Read("maxPauge", maxCategoryNum_);
    for (int32_t i = 0; i < arrowPositions_.size(); ++i) {
        _reader.Read<2, float>(("positions" + std::to_string(i)).c_str(), arrowPositions_[i]);
    }
}

void MenuStatus::Finalize() {
    // 必要ならリソース解放などを記述
}

void MenuStatus::OpenMenuAnimation(const float& time) {
    apperUVEasing_.time += time;

    scaleX_ = EaseInCubic(0.0f, 1.0f, apperUVEasing_.time, apperUVEasing_.maxTime);

    if (apperUVEasing_.time < apperUVEasing_.maxTime) {
        return;
    }
    scaleX_             = 1.0f;
    apperUVEasing_.time = 1.0f;
    /*  scrollStep_         = MenuMode::PAUGESELECTION;*/
}

void MenuStatus::CloseAnimation(const float& time) {
    apperUVEasing_.time += time;

    scaleX_ = EaseInCubic(1.0f, 0.0f, apperUVEasing_.time, apperUVEasing_.maxTime);

    if (apperUVEasing_.time < apperUVEasing_.maxTime) {
        return;
    }
    scaleX_             = 0.0f;
    apperUVEasing_.time = 0.0f;
    scrollStep_         = MenuMode::END;
}

void MenuStatus::SelectNextCategory() {
    int32_t next = static_cast<int32_t>(currentCategory_) + 1;
    if (next >= maxCategoryNum_) {
        next = 0;
    }
    currentCategory_ = static_cast<MenuCategory>(next);
    UpdateArrowPos();
}

void MenuStatus::SelectPreviousCategory() {
    int32_t prev = static_cast<int32_t>(currentCategory_) - 1;
    if (prev < 0) {
        prev = maxCategoryNum_ - 1;
    }
    currentCategory_ = static_cast<MenuCategory>(prev);
    UpdateArrowPos();
}

void MenuStatus::UpdateArrowPos() {
    arrowPos_ = arrowPositions_[static_cast<int32_t>(currentCategory_)];
}

void MenuStatus::Reset() {
    moveEasing_.time    = 0.0f;
    apperUVEasing_.time = 0.0f;
    scaleX_             = 0.0f;
    baseScale_          = {0.0f, 0.0f};
}

void MenuStatus::ScrollTimeReset() {
    apperUVEasing_.time = 0.0f;
  
}

void MenuStatus::DesideForCategory(Input* input) {

    if (!input->isTriggerKey(DIK_SPACE)) {
        return;
    }

    switch (currentCategory_) {
    case MenuStatus::MenuCategory::RETURNGAME:
        scrollStep_ = MenuMode::CLOSEINIT;
        break;
    case MenuStatus::MenuCategory::OPENTUTORIAL:
        scrollStep_ = MenuMode::WATCHINGTUTORIAL;
        break;
    case MenuStatus::MenuCategory::RETURNTITLE:
        scrollStep_ = MenuMode::END;
        break;
    default:
        break;
    }
}
