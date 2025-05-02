#pragma once

#include "component/IComponent.h"
#include <array>
#include <component/transform/Transform.h>
#include <cstdint>
#include <Entity.h>
#include <string>
#include <Vector3.h>

enum class EffectType {
    SCORE,
    TIME,
    MIMUSTIME,
};

enum class BlockEffectUIDigit {
    ICON,
    SIGN,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    COUNT,
};

class EffectByBlockUIStatus
    : public IComponent {
public:
private: // variables
    bool isAlive_ = true;

    float settingValue_    = 0.0f; // 取得した値
    EffectType effectType_ = EffectType::TIME; // エフェクトタイプ
    int32_t getDigitNum_;
    

    BlockEffectUIDigit digit_; // 整数の桁
    int32_t valueForDigit_;
    std::string currentTextureName_;
    float lifeTime_;

public:
    EffectByBlockUIStatus() {}
    virtual ~EffectByBlockUIStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

    int32_t GetValueForDigit();
    void SetCurerntIconTexture();
    void SetCurerntSignTexture();
    void SetCurerntNumberTexture(const int32_t& num);

    void DecrementCurrnetTime(const float&time);

    void EffectUISpawn(GameEntity* _entity,const float&value,const EffectType&type);

public: // accsessor
    /// getter
    BlockEffectUIDigit GetDigit() const { return digit_; }
    EffectType GetEffectType() const { return effectType_; }
    std::string GetCurrentTextureName() const { return currentTextureName_; }
    float GetLifeTime() const { return lifeTime_; }
    /// setter
    void SetcurrentTextureName(const std::string& currentTextureName) { currentTextureName_ = currentTextureName; }
    void SetValue(const float& scoreChange) { settingValue_ = scoreChange; }
    void SetEffectType(const EffectType& type) { effectType_ = type; }
    void SetDigit(const BlockEffectUIDigit& type) { digit_ = type; }
    void SetLifeTime(const float& time) { lifeTime_ = time; }
};
