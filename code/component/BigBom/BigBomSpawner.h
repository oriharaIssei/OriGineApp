#pragma once

#include "component/IComponent.h"
#include <Vector3.h>


class BigBomSpawner
    : public IComponent {

private: // variables

    /// frag
    bool isAlive_ = true;
    bool isPut_;            // 設置フラグ

    /// num
    int32_t ableSetBomNum_; // 設置爆弾数
    int32_t putTotalNum_;    // 設置中の数

    /// time
    float putCoolTimeMax_;   // 次に置くまでのクールタイム
    float currentPutCoolTime_;

    /// offset
    Vec3f spawnOffset_;

    Vec3f collisionCenter_;
    float collisionRadius_;

     float launthSpeed_;

public:
    BigBomSpawner() {}
    virtual ~BigBomSpawner() = default;

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
    int32_t GetAblePutNum() const{ return ableSetBomNum_; }
    int32_t GetPutTotalNum() const { return putTotalNum_; }
    Vec3f GetSpawnOffset() const { return spawnOffset_; }
    Vec3f GetCollisionCenter() const { return collisionCenter_; }
    float GetCollisionRadius() const { return collisionRadius_; }
    float GetLaunghSpeed() const { return launthSpeed_; }
    /// setter
    void SetIsPut(const bool& is) {  isPut_ = is; }
    void SetPutCurrentCoolTime(const float& time) { currentPutCoolTime_ = time; }
    void SetIncrementTotalNum() { putTotalNum_++; }
    void SetDecrementPuttingNum() { putTotalNum_--; }
};
