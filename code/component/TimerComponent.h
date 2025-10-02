#pragma once
#include "component/IComponent.h"

/// math
#include <math/Vector2.h>

class TimerComponent
    : public IComponent {
    friend void to_json(nlohmann::json& j, const TimerComponent& c);
    friend void from_json(const nlohmann::json& j, TimerComponent& c);

public:
    TimerComponent();
    ~TimerComponent() override;

    void Initialize(GameEntity* _entity) override;
    void Edit(Scene* _scene, GameEntity* _entity, const std::string& _parentLabel) override;
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
    void setStarted(bool started) { isStarted_ = started; }

    float getMaxTime() const { return maxTime_; }
    void setMaxTime(float time) { maxTime_ = time; }

    float getCurrentTime() const { return currentTime_; }
    void setCurrentTime(float time) { currentTime_ = time; }

    void resetCurrentTime() { currentTime_ = maxTime_; }
};

class TimerForSpriteComponent
    : public IComponent {
    friend void to_json(nlohmann::json& j, const TimerForSpriteComponent& c);
    friend void from_json(const nlohmann::json& j, TimerForSpriteComponent& c);

public:
    TimerForSpriteComponent()           = default;
    ~TimerForSpriteComponent() override = default;

    void Initialize(GameEntity* _entity) override;
    void Edit(Scene* _scene, GameEntity* _entity, const std::string& _parentLabel) override;
    void Finalize() override {}

private:
    void settingOnLoadTexture(uint32_t _texIdx);

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
    int32_t getSpritesEntityId() const { return spritesEntityId_; }
    void setSpritesEntityId(int32_t id) { spritesEntityId_ = id; }

    const std::string& getNumbersTexturePath() const { return numbersTexturePath_; }
    uint32_t getTextureIndex() const { return textureIndex_; }

    const Vec2f& getNumbersTextureSize() const { return numbersTextureSize_; }
    const Vec2f& getNumberTileSize() const { return numberTileSize_; }
    void setNumbersTexturePath(const std::string& path) { numbersTexturePath_ = path; }

    const Vec2f& getAnchorOnWindow() const { return anchorOnWindow_; }
    void setAnchorOnWindow(const Vec2f& anchor) { anchorOnWindow_ = anchor; }

    const Vec2f& getOffset() const { return offset_; }
    void setOffset(const Vec2f& offset) { offset_ = offset; }

    const Vec2f& getMarginBetweenIntegerAndDecimal() const { return marginBetweenIntegerAndDecimal_; }
    void setMarginBetweenIntegerAndDecimal(const Vec2f& margin) { marginBetweenIntegerAndDecimal_ = margin; }

    const Vec2f& getSpriteSizeInteger() const { return spriteSizeInteger_; }
    void setSpriteSizeInteger(const Vec2f& size) { spriteSizeInteger_ = size; }

    const Vec2f& getSpriteSizeDecimal() const { return spriteSizeDecimal_; }
    void setSpriteSizeDecimal(const Vec2f& size) { spriteSizeDecimal_ = size; }

    const Vec2f& getSpriteMarginInteger() const { return spriteMarginInteger_; }
    void setSpriteMarginInteger(const Vec2f& margin) { spriteMarginInteger_ = margin; }

    const Vec2f& getSpriteMarginDecimal() const { return spriteMarginDecimal_; }
    void setSpriteMarginDecimal(const Vec2f& margin) { spriteMarginDecimal_ = margin; }

    int getDigitForSprite() const { return digitForSprite_; }
    void setDigitForSprite(int digit) { digitForSprite_ = digit; }

    int getDigitIntegerForSprite() const { return digitIntegerForSprite_; }
    void setDigitIntegerForSprite(int digit) { digitIntegerForSprite_ = digit; }

    int getDigitDecimalForSprite() const { return digitDecimalForSprite_; }
    void setDigitDecimalForSprite(int digit) { digitDecimalForSprite_ = digit; }
};
