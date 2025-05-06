#pragma once

#include "component/IComponent.h"
#include <array>
#include <component/transform/Transform.h>
#include <cstdint>
#include <Entity.h>
#include <Vector3.h>

class ScoreStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json,const ScoreStatus& _component);
    friend void from_json(const nlohmann::json& _json,ScoreStatus& _component);

private: // variables
    bool isAlive_ = true;

   static float currentScore_;
    float pulusScore_ = 0.0f;
    float scoreChangeTime_=0.0f;
    float scoreMax_  = 99999.0f;
    bool isChanging_ = false;

public:
    ScoreStatus() {}
    virtual ~ScoreStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

   void PlusScoreIncrement(const float&value);
    void TimeIncrement(const float& value);

public: // accsessor
    /// getter
    float GetCurrentScore() const { return currentScore_; }
    float GetPulusScore() const { return pulusScore_; }
    float GetScoreChangeTIme() const { return scoreChangeTime_; }
    const float GetScoreMax() const { return scoreMax_; }
  
    /// setter
    void SetScoreChangeTime(const float& scoreChange) {  scoreChangeTime_ = scoreChange; }
    void SetCurrentScore(const float& scoreChange) { currentScore_ = scoreChange; }
};
