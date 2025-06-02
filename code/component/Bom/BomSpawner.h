#pragma once

#include "component/IComponent.h"


class BomSpawner
    : public IComponent {
    friend void to_json(nlohmann::json& _json,const BomSpawner& _bomSpawner);
    friend void from_json(const nlohmann::json& _json, BomSpawner& _bomSpawner);

private: // variables

    /// frag
    bool isAlive_ = true;
    bool isLaunch_=false;            // 設置フラグ
    bool isLaunched_=false;

    /// num
    int32_t ableSetBomNum_; // 設置爆弾数
    int32_t putTotalNum_;    // 設置中の数

    /// time
    float putCoolTimeMax_;   // 次に置くまでのクールタイム
    float currentPutCoolTime_;

public:
    BomSpawner() {}
    virtual ~BomSpawner() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

public: // accsessor
    /// getter
  
    float GetPutCoolTimeMax() const { return putCoolTimeMax_; }
    float GetCurrenPutCoolTime() const { return currentPutCoolTime_; } 
    bool GetIsLaunch() const { return isLaunch_; }
    bool GetIsLaunched() const { return isLaunched_; }
    int32_t GetAblePutNum() const{ return ableSetBomNum_; }
    int32_t GetPutTotalNum() const { return putTotalNum_; }
    /// setter
    void SetIsLaunch(const bool& is) {  isLaunch_ = is; }
    void SetIsLaunched(const bool& is) { isLaunched_ = is; }
    void SetPutCurrentCoolTime(const float& time) { currentPutCoolTime_ = time; }
    void SetIncrementTotalNum() { putTotalNum_++; }
    void SetDecrementPuttingNum() { putTotalNum_--; }
};
