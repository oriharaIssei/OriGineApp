#include "EffectByBlockUIStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void EffectByBlockUIStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool EffectByBlockUIStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    //// ComboDigit（桁数）のUI選択
    // static const char* digitLabels[] = {
    //     "ONE",
    //     "TWO",
    //     "THREE",
    //     "FOUR",
    //     "FIVE",
    // };
    // int currentIndex                 = static_cast<int>(digit_);
    // if (ImGui::Combo("Timer Digit", &currentIndex, digitLabels, static_cast<int>(UIDigit::COUNT))) {
    //     digit_ = static_cast<UIDigit>(currentIndex);
    // }

    return isChange;
}

void EffectByBlockUIStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    /* _writer.Write("timerDigit", static_cast<int32_t>(digit_));*/
}

void EffectByBlockUIStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    /*  int32_t digit = 0;
      _reader.Read("timerDigit", digit);
      digit_ = static_cast<UIDigit>(digit);*/
}

void EffectByBlockUIStatus::Finalize() {}

int32_t EffectByBlockUIStatus::GetValueForDigit() {
    switch (digit_) {
    case UIDigit::ONE: {
        // 小数点以下第1位を取得
        int32_t intValue = static_cast<int32_t>(settingValue_) % 10;
        return intValue;
    }
    case UIDigit::TWO: {
        // 整数部の第2位を取得
        int32_t intValue = static_cast<int32_t>(settingValue_) / 10 % 10;
        return intValue;
    }
    case UIDigit::THREE: {
        // 整数部の第2位を取得
        int32_t intValue = static_cast<int32_t>(settingValue_) / 100 % 10;
        return intValue;
    }
    case UIDigit::FOUR: {
        // 整数部の第2位を取得
        int32_t intValue = static_cast<int32_t>(settingValue_) / 1000 % 10;
        return intValue;
    }
    case UIDigit::FIVE: {
        // 整数部の第2位を取得
        int32_t intValue = static_cast<int32_t>(settingValue_) / 10000 % 10;
        return intValue;
    }

    default:
        return 0;
    }
}

void EffectByBlockUIStatus::SetCurerntIconTexture() {

    switch (effectType_) {
    case EffectType::SCORE:
        currentTextureName_ ="";
        break;
    case EffectType::TIME:
        currentTextureName_ = "/Texture/UI/TimeIcom.png";
        break;
    case EffectType::MIMUSTIME:
        currentTextureName_ = "/Texture/UI/TimeIcom.png";
        break;
    default:
        break;
    }

}

void EffectByBlockUIStatus::SetCurerntSignTexture() {

    switch (effectType_) {
    case EffectType::SCORE:
        currentTextureName_ = "";
        break;
    case EffectType::TIME:
        currentTextureName_ = "/Texture/UI/AddTimeUI.png";
        break;
    case EffectType::MIMUSTIME:
        currentTextureName_ = "/Texture/UI/SubtractTimeUI.png";
        break;
    default:
        break;
    }
}

void EffectByBlockUIStatus::SetCurerntNumberTexture(const int32_t&num) {
   
    switch (effectType_) {
    case EffectType::SCORE:
        currentTextureName_ = ("/Texture/UI/Numbers/Number" + std::to_string(num) +".png").c_str();
        break;
    case EffectType::TIME:
        currentTextureName_ = ("/Texture/UI/GreenNumbers/greennumber" + std::to_string(num) + ".png").c_str();
        break;
    case EffectType::MIMUSTIME:
        currentTextureName_ = ("/Texture/UI/BlueNumbers/bluenumber" + std::to_string(num) + ".png").c_str();
        break;
    default:
        break;
    }
}

void EffectByBlockUIStatus::DecrementCurrnetTime(const float& time) {
    lifeTime_ -= time;
  }
