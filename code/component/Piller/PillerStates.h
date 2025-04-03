#pragma once

#include "component/IComponent.h"
#include <cstdint>

class PillerStates
    : public IComponent {

private: // variables

    bool isAlive_ = true;
    bool isBroken_ = false;

     int32_t HPMax_;
    int32_t currentHP_;
    float collisionSize_;

     int32_t columNum_;
     int32_t rowNum_;

public:
    PillerStates() {}
    virtual ~PillerStates() = default;

    void Initialize(GameEntity* _entity) override;

    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

    void TakeDamage();

public: // accsessor
    /// getter
    float GetCollisionSize() const { return collisionSize_; }
    int32_t GetHpMax() const { return HPMax_; }
    int32_t GetCurrentHp() const { return currentHP_; }
    int32_t GetColumNum() const { return columNum_; }
    int32_t GetRowNum() const { return rowNum_; }
    /// setter
    void SetCurrentHp(const int32_t& hp) { currentHP_ = hp; }
    void SetColumAndRow(const int32_t& colum, const int32_t& row);
 
};
