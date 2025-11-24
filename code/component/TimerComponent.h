#pragma once
#include "component/IComponent.h"

/// math
#include <math/Vector2.h>

/// <summary>
/// Timerコンポーネント
/// </summary>
class TimerComponent
    : public IComponent {
    friend void to_json(nlohmann::json& j, const TimerComponent& c);
    friend void from_json(const nlohmann::json& j, TimerComponent& c);

public:
    TimerComponent();
    ~TimerComponent() override;

    void Initialize(Entity* _entity) override;
    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
    void Finalize() override;

private:
    /// <summary>
    /// システムで使用を開始したかどうか
    /// </summary>
    bool isStarted_ = false;

    float maxTime_     = 0.f;
    float currentTime_ = 0.f;

    Vec2f spriteSize_;

public:
    bool isStarted() const { return isStarted_; }
    void SetStarted(bool started) { isStarted_ = started; }

    float GetMaxTime() const { return maxTime_; }
    void SetMaxTime(float time) { maxTime_ = time; }

    float GetTime() const { return currentTime_; }
    void SetCurrentTime(float time) { currentTime_ = time; }

    void ResetCurrentTime() { currentTime_ = maxTime_; }
};

class TimerForSpriteComponent
    : public IComponent {
    friend void to_json(nlohmann::json& j, const TimerForSpriteComponent& c);
    friend void from_json(const nlohmann::json& j, TimerForSpriteComponent& c);

public:
    TimerForSpriteComponent()           = default;
    ~TimerForSpriteComponent() override = default;

    void Initialize(Entity* _entity) override;
    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
    void Finalize() override {}

private:
    void SettingOnLoadTexture(uint32_t _texIdx);

private:
    int32_t spritesEntityId_ = -1; // スプライトを持つエンティティID

    int32_t digitForSprite_        = 0; // 表示する桁数
    int32_t digitIntegerForSprite_ = 0; // 整数部の桁数
    int32_t digitDecimalForSprite_ = 0; // 小数部の桁数

    uint32_t textureIndex_          = 0; // 数字のテクスチャインデックス
    std::string numbersTexturePath_ = ""; // 数字のテクスチャ名
    Vec2f numbersTextureSize_       = {0.f, 0.f}; // 数字のテクスチャサイズ
    Vec2f numberTileSize_           = {0.f, 0.f}; // 数字のタイルサイズ

    Vec2f anchorOnWindow_ = {0.5f, 0.2f}; // ウィンドウ上のアンカー位置
    Vec2f offset_         = {0.f, 0.f}; // オフセット位置

    // 整数部・小数部のスプライトサイズ
    Vec2f spriteSizeInteger_ = {0.f, 0.f}; // 整数部のスプライトサイズ
    Vec2f spriteSizeDecimal_ = {0.f, 0.f}; // 小数部のスプライトサイズ

    // 整数部・小数部のスプライトマージン
    Vec2f spriteMarginInteger_ = {0.f, 0.f}; // 整数部のスプライトマージン
    Vec2f spriteMarginDecimal_ = {0.f, 0.f}; // 小数部のスプライトマージン

    // 整数部と小数部の間のスペース
    Vec2f marginBetweenIntegerAndDecimal_ = {0.f, 0.f}; // 整数部と小数部の間のスペース

public:
    int32_t GetSpritesEntityId() const { return spritesEntityId_; }
    void SetSpritesEntityId(int32_t id) { spritesEntityId_ = id; }

    const std::string& GetNumbersTexturePath() const { return numbersTexturePath_; }
    uint32_t GetTextureIndex() const { return textureIndex_; }

    const Vec2f& GetNumbersTextureSize() const { return numbersTextureSize_; }
    const Vec2f& GetNumberTileSize() const { return numberTileSize_; }
    void SetNumbersTexturePath(const std::string& path) { numbersTexturePath_ = path; }

    const Vec2f& GetAnchorOnWindow() const { return anchorOnWindow_; }
    void SetAnchorOnWindow(const Vec2f& anchor) { anchorOnWindow_ = anchor; }

    const Vec2f& GetOffset() const { return offset_; }
    void SetOffset(const Vec2f& offset) { offset_ = offset; }

    const Vec2f& GetMarginBetweenIntegerAndDecimal() const { return marginBetweenIntegerAndDecimal_; }
    void SetMarginBetweenIntegerAndDecimal(const Vec2f& margin) { marginBetweenIntegerAndDecimal_ = margin; }

    const Vec2f& GetSpriteSizeInteger() const { return spriteSizeInteger_; }
    void SetSpriteSizeInteger(const Vec2f& size) { spriteSizeInteger_ = size; }

    const Vec2f& GetSpriteSizeDecimal() const { return spriteSizeDecimal_; }
    void SetSpriteSizeDecimal(const Vec2f& size) { spriteSizeDecimal_ = size; }

    const Vec2f& GetSpriteMarginInteger() const { return spriteMarginInteger_; }
    void SetSpriteMarginInteger(const Vec2f& margin) { spriteMarginInteger_ = margin; }

    const Vec2f& GetSpriteMarginDecimal() const { return spriteMarginDecimal_; }
    void SetSpriteMarginDecimal(const Vec2f& margin) { spriteMarginDecimal_ = margin; }

    int GetDigitForSprite() const { return digitForSprite_; }
    void SetDigitForSprite(int digit) { digitForSprite_ = digit; }

    int GetDigitIntegerForSprite() const { return digitIntegerForSprite_; }
    void SetDigitIntegerForSprite(int digit) { digitIntegerForSprite_ = digit; }

    int GetDigitDecimalForSprite() const { return digitDecimalForSprite_; }
    void SetDigitDecimalForSprite(int digit) { digitDecimalForSprite_ = digit; }
};
