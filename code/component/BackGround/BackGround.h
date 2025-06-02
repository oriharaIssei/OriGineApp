#pragma once

#include "component/IComponent.h"
#include <array>
#include <component/transform/Transform.h>
#include <cstdint>
#include <Entity.h>
#include <Vector3.h>

class ScoreStatus
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    float currentScore_;
    float pulusScore_ = 0.0f;
    float scoreChangeTime_=0.0f;
    bool isChanging_ = false;

public:
    ScoreStatus() {}
    virtual ~ScoreStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

   void PlusScoreIncrement(const float&value);
    void TimeIncrement(const float& value);

public: // accsessor
    /// getter
    float GetCurrentScore() const { return currentScore_; }
    float GetPulusScore() const { return pulusScore_; }
    float GetScoreChangeTIme() const { return scoreChangeTime_; }
  
    /// setter
    void SetScoreChangeTime(const float& scoreChange) {  scoreChangeTime_ = scoreChange; }
    void SetCurrentScore(const float& scoreChange) { currentScore_ = scoreChange; }
};
