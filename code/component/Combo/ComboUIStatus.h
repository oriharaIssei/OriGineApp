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
    ComboDigit comboDigit_; //コンボの桁
  
public:
    ComboUIStatus() {}
    virtual ~ComboUIStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

    

public: // accsessor
    /// getter
    ComboDigit GetComboDidit()const  { return comboDigit_ };
    /// setter
   
};
