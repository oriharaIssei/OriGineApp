#include "GameStartStatus.h"

// 外部ライブラリなど
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void GameStartStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    // 必要であれば初期化処理をここに記述
}

bool GameStartStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG
    isChange |= CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();
    ImGui::Text("Position - Purpose");
    isChange |= DragGuiVectorCommand("purposeStartPos_", purposeStartPos_, 0.01f);
    isChange |= DragGuiVectorCommand("purposeEndPos_", purposeEndPos_, 0.01f);
    isChange |= DragGuiVectorCommand("purposeClosePos_", purposeClosePos_, 0.01f);

    ImGui::Spacing();
    ImGui::Text("Position - Ready");
    isChange |= DragGuiVectorCommand("readyStartPos_", readyStartPos_, 0.01f);
    isChange |= DragGuiVectorCommand("readyEndPos_", readyEndPos_, 0.01f);

    ImGui::Spacing();
    ImGui::Text("ScaleAndRotate - Go");
    isChange |= DragGuiVectorCommand("goStartScale_", goStartScale_, 0.01f);
    isChange |= DragGuiVectorCommand("goEndScale_", goEndScale_, 0.01f);
    isChange |= DragGuiCommand("goRotateSpeed", goRotateSpeed_, 0.01f);

    ImGui::Spacing();
    ImGui::Text("Time");
    isChange |= DragGuiCommand("firstWaitTime_", firstWaitTime_, 0.01f);
    isChange |= DragGuiCommand("waitTimeAfterApear_", waitTimeAfterApear_, 0.01f);
    isChange |= DragGuiCommand("waitTimeAfterClose_", waitTimeAfterClose_, 0.01f);
    isChange |= DragGuiCommand("waitTimeAfterGo_", waitTimeAfterGo_, 0.01f);

    ImGui::Spacing();
    ImGui::Text("Easing - Apear");
    isChange |= DragGuiCommand("apearEasing_.maxTime", apearEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("apearEasing_.backRatio", apearEasing_.backRatio, 0.01f);
    isChange |= DragGuiCommand("apearEasing_.amplitude", apearEasing_.amplitude, 0.01f);
    isChange |= DragGuiCommand("apearEasing_.period", apearEasing_.period, 0.01f);

    ImGui::Text("Easing - Close");
    isChange |= DragGuiCommand("closeEasing_.maxTime", closeEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("closeEasing_.backRatio", closeEasing_.backRatio, 0.01f);
    isChange |= DragGuiCommand("closeEasing_.amplitude", closeEasing_.amplitude, 0.01f);
    isChange |= DragGuiCommand("closeEasing_.period", closeEasing_.period, 0.01f);

    ImGui::Text("Easing - Ready");
    isChange |= DragGuiCommand("readyEasing_.maxTime", readyEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("readyEasing_.backRatio", readyEasing_.backRatio, 0.01f);
    isChange |= DragGuiCommand("readyEasing_.amplitude", readyEasing_.amplitude, 0.01f);
    isChange |= DragGuiCommand("readyEasing_.period", readyEasing_.period, 0.01f);

    ImGui::Text("Easing - Go");
    isChange |= DragGuiCommand("goEasing_.maxTime", goEasing_.maxTime, 0.01f);

    ImGui::Text("Easing - GoBack");
    isChange |= DragGuiCommand("goBackEasing_.maxTime", goBackEasing_.maxTime, 0.01f);

#endif
    return isChange;
}

void GameStartStatus::ApearEasing(const float& deltaTime) {
    apearEasing_.time += deltaTime;

    /// 　ease
    purposePos_ = EaseOutBack(purposeStartPos_, purposeEndPos_, apearEasing_.time, apearEasing_.maxTime);

    // end
    if (apearEasing_.time < apearEasing_.maxTime) {
        return;
    }

    // adapt
    apearEasing_.time = apearEasing_.maxTime;
    purposePos_       = purposeEndPos_;
    renditionStep_    = RenditionStep::WAITAFTERAPERAR;
}

void GameStartStatus::CloseEasing(const float& deltaTime) {
    closeEasing_.time += deltaTime;

    /// 　ease
    purposePos_ = EaseInBack(purposeEndPos_, purposeClosePos_, closeEasing_.time, closeEasing_.maxTime);

    // end
    if (closeEasing_.time < closeEasing_.maxTime) {
        return;
    }

    // adapt
    closeEasing_.time = closeEasing_.maxTime;
    purposePos_       = purposeClosePos_;
    renditionStep_    = RenditionStep::WAITAFTERCLOSE;
}

void GameStartStatus::ReadyEasing(const float& deltaTime) {
    readyEasing_.time += deltaTime;

    /// 　ease
    readyPos_ = EaseInCubic(readyStartPos_, readyEndPos_, readyEasing_.time, readyEasing_.maxTime);

    // end
    if (readyEasing_.time < readyEasing_.maxTime) {
        return;
    }

    // adapt
    readyEasing_.time = readyEasing_.maxTime;
    purposePos_       = purposeClosePos_;
    renditionStep_    = RenditionStep::GO;
}

void GameStartStatus::GoEasing(const float& deltaTime) {
    goEasing_.time += deltaTime;
    goRotate_ += goRotateSpeed_;

    /// 　ease
    goScale_ = EaseInQuart(goStartScale_, goEndScale_, goEasing_.time, goEasing_.maxTime);

    // end
    if (goEasing_.time < goEasing_.maxTime) {
        return;
    }

    // adapt
    goEasing_.time = goEasing_.maxTime;
    goScale_       = goEndScale_;
    goRotate_      = 0.0f;
    renditionStep_ = RenditionStep::WAITAFTERGO;
}

void GameStartStatus::GoBackEasing(const float& deltaTime) {
    goBackEasing_.time += deltaTime;
    goRotate_ -= goRotateSpeed_;

    /// 　ease
    goScale_ = EaseInBack(goEndScale_, Vec2f(0.0f, 0.0f), goBackEasing_.time, goBackEasing_.maxTime);
    readyScale_ = EaseInBack(Vec2f(1.0f, 1.0f), Vec2f(0.0f, 0.0f), goBackEasing_.time, goBackEasing_.maxTime);

    // end
    if (goBackEasing_.time < goBackEasing_.maxTime) {
        return;
    }

    // adapt
    goBackEasing_.time = goBackEasing_.maxTime;
    goScale_       = Vec2f(0.0f, 0.0f);
    readyScale_        = Vec2f(0.0f, 0.0f);
    goRotate_      = 0.0f;
    renditionStep_ = RenditionStep::END;
}


void GameStartStatus::Reset() {
    goEasing_.time    = 0.0f;
    readyEasing_.time = 0.0f;
    closeEasing_.time = 0.0f;
    apearEasing_.time = 0.0f;
    goBackEasing_.time = 0.0f;

    readyScale_ = Vec2f(1.0f, 1.0f);

    goRotate_   = 0.0f;
    goScale_    = goStartScale_;
    readyPos_   = readyStartPos_;
    purposePos_ = purposeStartPos_;
}

void to_json(nlohmann::json& j, const GameStartStatus& m) {
    j["isAlive"] = m.isAlive_;

    j["firstWaitTime"]      = m.firstWaitTime_;
    j["waitTimeAfterApear"] = m.waitTimeAfterApear_;
    j["waitTimeAfterClose"] = m.waitTimeAfterClose_;
    j["waitTimeAfterGo"] = m.waitTimeAfterGo_;

    j["purposeStartPos"] = m.purposeStartPos_;
    j["purposeEndPos"]   = m.purposeEndPos_;
    j["purposeClosePos"] = m.purposeClosePos_;

    j["readyStartPos"] = m.readyStartPos_;
    j["readyEndPos"]   = m.readyEndPos_;

    j["goStartScale"]  = m.goStartScale_;
    j["goEndScale"]    = m.goEndScale_;
    j["goRotateSpeed"] = m.goRotateSpeed_;

    // Easing
    j["apearEasing.maxTime"]   = m.apearEasing_.maxTime;
    j["apearEasing.backRatio"] = m.apearEasing_.backRatio;
    j["apearEasing.amplitude"] = m.apearEasing_.amplitude;
    j["apearEasing.period"]    = m.apearEasing_.period;

    j["closeEasing.maxTime"]   = m.closeEasing_.maxTime;
    j["closeEasing.backRatio"] = m.closeEasing_.backRatio;
    j["closeEasing.amplitude"] = m.closeEasing_.amplitude;
    j["closeEasing.period"]    = m.closeEasing_.period;

    j["goEasing.maxTime"]   = m.goEasing_.maxTime;
    j["goEasing.backRatio"] = m.goEasing_.backRatio;
    j["goEasing.amplitude"] = m.goEasing_.amplitude;
    j["goEasing.period"]    = m.goEasing_.period;

    j["readyEasing.maxTime"]   = m.readyEasing_.maxTime;
    j["readyEasing.backRatio"] = m.readyEasing_.backRatio;
    j["readyEasing.amplitude"] = m.readyEasing_.amplitude;
    j["readyEasing.period"]    = m.readyEasing_.period;

    j["goBackEasing.maxTime"] = m.goBackEasing_.maxTime;
}

void from_json(const nlohmann::json& j, GameStartStatus& m) {
    j.at("isAlive").get_to(m.isAlive_);

    j.at("firstWaitTime").get_to(m.firstWaitTime_);
    j.at("waitTimeAfterApear").get_to(m.waitTimeAfterApear_);
    j.at("waitTimeAfterClose").get_to(m.waitTimeAfterClose_);
    j.at("waitTimeAfterGo").get_to(m.waitTimeAfterGo_);

    j.at("purposeStartPos").get_to(m.purposeStartPos_);
    j.at("purposeEndPos").get_to(m.purposeEndPos_);
    j.at("purposeClosePos").get_to(m.purposeClosePos_);

    j.at("readyStartPos").get_to(m.readyStartPos_);
    j.at("readyEndPos").get_to(m.readyEndPos_);

    j.at("goStartScale").get_to(m.goStartScale_);
    j.at("goEndScale").get_to(m.goEndScale_);
    j.at("goRotateSpeed").get_to(m.goRotateSpeed_);

    // Easing
    j.at("apearEasing.maxTime").get_to(m.apearEasing_.maxTime);
    j.at("apearEasing.backRatio").get_to(m.apearEasing_.backRatio);
    j.at("apearEasing.amplitude").get_to(m.apearEasing_.amplitude);
    j.at("apearEasing.period").get_to(m.apearEasing_.period);

    j.at("closeEasing.maxTime").get_to(m.closeEasing_.maxTime);
    j.at("closeEasing.backRatio").get_to(m.closeEasing_.backRatio);
    j.at("closeEasing.amplitude").get_to(m.closeEasing_.amplitude);
    j.at("closeEasing.period").get_to(m.closeEasing_.period);

    j.at("goEasing.maxTime").get_to(m.goEasing_.maxTime);
    j.at("goEasing.backRatio").get_to(m.goEasing_.backRatio);
    j.at("goEasing.amplitude").get_to(m.goEasing_.amplitude);
    j.at("goEasing.period").get_to(m.goEasing_.period);

    j.at("readyEasing.maxTime").get_to(m.readyEasing_.maxTime);
    j.at("readyEasing.backRatio").get_to(m.readyEasing_.backRatio);
    j.at("readyEasing.amplitude").get_to(m.readyEasing_.amplitude);
    j.at("readyEasing.period").get_to(m.readyEasing_.period);

    j.at("goBackEasing.maxTime").get_to(m.goBackEasing_.maxTime);
}

void GameStartStatus::Finalize() {
    // 必要ならリソース解放などを記述
}
