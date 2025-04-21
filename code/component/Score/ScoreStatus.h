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

    int32_t currentScore_;
    float pulusScore_ = 0.0f;
  
    bool isChanging_ = false;

public:
    ScoreStatus() {}
    virtual ~ScoreStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

   

public: // accsessor
    /// getter
    float GetCurrentScore() const { return currentScore_; }
    float GetPulusScore() const { return pulusScore_; }
   
  
    /// setter
    void SetCurrentTimer(float currentTimer) { currentTimer_ = currentTimer; }
   
};
