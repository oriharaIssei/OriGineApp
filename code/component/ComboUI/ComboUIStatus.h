#pragma once

#include "component/IComponent.h"
#include <cstdint>
#include <Entity.h>
#include <component/transform/Transform.h>
#include <Vector3.h>
#include<array>

class ComboUIStatus
    : public IComponent {
public:
    enum class ComboDigit {
        ONE,
        TWO,
        THREE,
        COUNT,
    };

private: // variables
    bool isAlive_ = true;

    int32_t currentComboNum_ = 0; // 現在のコンボ数
    std::array<int32_t, static_cast<size_t>(ComboDigit::COUNT)> comboDigit_;

public:
    ComboUIStatus() {}
    virtual ~ComboUIStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

      int32_t GetComboDidit(const ComboDigit& _ditit);

public: // accsessor
    /// getter
 
    /// setter
   
};
