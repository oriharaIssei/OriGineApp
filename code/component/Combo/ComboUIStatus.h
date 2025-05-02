#pragma once

#include "component/IComponent.h"
#include <cstdint>
#include <Entity.h>
#include <component/transform/Transform.h>
#include <Vector3.h>
#include<array>
#include<string>

class ComboUIStatus
    : public IComponent {
public:
    enum class TimeDigit {
        ONE,
        TWO,
        THREE,
        COUNT,
    };

private: // variables
    bool isAlive_ = true;
    TimeDigit timeDigit_; //コンボの桁
    int32_t valueForDigit_;
    std::string currentTextureName_;
  
public:
    ComboUIStatus() {}
    virtual ~ComboUIStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

    int32_t GetValueForDigit(const int32_t&value);

public: // accsessor
    /// getter
    TimeDigit GetTimeDigit() const { return timeDigit_; }
    std::string GetCurrentTextureName() const { return currentTextureName_; }
    /// setter
    void SetcurrentTextureName(const std::string& currentTextureName) { currentTextureName_ = currentTextureName; }
};
