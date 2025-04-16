#pragma once

#include "component/IComponent.h"
#include<array>
#include <cstdint>
#include<Vector3.h>

class FloorAndPillerrStatus
    : public IComponent {

private: // variables

    bool isAlive_ = true;

    bool isFall_ = false;
   /* bool isDestroy_=false;*/
 

    int32_t columNum_;
    int32_t rowNum_;
    float savePosY_;
    float fallValue_;
    float fallPosY_;
    float fallspeed_;
    float fallEaseT_;
    int32_t currentHP_;
    bool isDestroy_ = false;
    

public:
    FloorAndPillerrStatus() {}
    virtual ~FloorAndPillerrStatus() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Save(BinaryWriter& _writer) override;
    void Load(BinaryReader& _reader) override;
    void Finalize() override;

    void TakeDamage();

  
 void SetColumDecrement();
   

public: // accsessor

    /// getter
    int32_t GetColumNum() const { return columNum_; }
    int32_t GetRowNum() const { return rowNum_; }
    int32_t GetCurrentHP() const { return currentHP_; }
    float GetSavePos() const { return savePosY_; }
    float GetFallValue() const { return fallValue_; }
    float GetFallPosY();
    float GetFallEaseT() const { return fallEaseT_; }
    float GetFallSpeed() const { return fallspeed_; }
    bool GetIsDestroy() const { return isDestroy_; }
    const bool& GetIsFall() const { return isFall_; }

      /// setter
    void SetColumAndRow(const int32_t& colum, const int32_t& row);
    void SetIsFall(const bool& is) { isFall_ = is; }
    void SetSavePos(const float& pos) { savePosY_ = pos; }
    void SetFallValue(const float& value) { fallValue_ = value; }
    void SetFallPosY(const float& pos) { fallPosY_ = pos; }
    void SetFallEaseT(const float& t) { fallEaseT_ = t; }
    void SetIncrementFallEaseT(const float& t) { fallEaseT_ += t; }
    void SetFallSpeed(const float& speed) { fallspeed_ = speed; }
    void SetcurrentHP(const int32_t& hp) { currentHP_ = hp; }
    void SetIsDestroy(const bool& is) { isDestroy_ = is;}

    /* void SetIsDestroyPointer(bool* ptr) {
        isDestroy_ = ptr;
    }*/
};
