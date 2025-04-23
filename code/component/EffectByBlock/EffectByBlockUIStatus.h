#pragma once

#include "component/IComponent.h"
#include <array>
#include <component/transform/Transform.h>
#include <cstdint>
#include <Entity.h>
#include <string>
#include <Vector3.h>

class EffectByBlockUIStatus
    : public IComponent {
public:
    enum class TimeDigit {
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        COUNT,
    };

private: // variables
    bool isAlive_ = true;
    TimeDigit digit_;        // 整数の桁
    int32_t valueForDigit_;
    std::string currentTextureName_;

public:
    EffectByBlockUIStatus() {}
    virtual ~EffectByBlockUIStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

    int32_t GetValueForDigit(const float& value);

public: // accsessor
    /// getter
    TimeDigit GetDigit() const { return digit_; }
    std::string GetCurrentTextureName() const { return currentTextureName_; }
    /// setter
    void SetcurrentTextureName(const std::string& currentTextureName) { currentTextureName_ = currentTextureName; }
};
