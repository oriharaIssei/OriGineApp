#pragma once

#include "component/IComponent.h"


class BomSpawner
    : public IComponent {

private: // variables

    /// frag
    bool isAlive_ = true;
    bool isPut_;            // 設置フラグ

    /// num
    int32_t ableSetBomNum_; // 設置爆弾数
    int32_t puttingNum_;    // 設置中の数

    /// time
    float putCoolTimeMax_;   // 次に置くまでのクールタイム
    float currentPutCoolTime_;

public:
    BomSpawner() {}
    virtual ~BomSpawner() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

public: // accsessor
    /// getter
  
    float GetPutCoolTimeMax() const { return putCoolTimeMax_; }
    float GetCurrenPutCoolTime() const { return currentPutCoolTime_; } 
    bool GetIsPut() const { return isPut_; }
    /// setter
    void SetIsPut(const bool& is) {  isPut_ = is; }
    void SetPutCurrentCoolTime(const float& time) { currentPutCoolTime_ = time; }
};
