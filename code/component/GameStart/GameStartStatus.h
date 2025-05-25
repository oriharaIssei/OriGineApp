#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"

#include <Entity.h>
#include <Vector2.h>

// class BlockManager;
class GameStartStatus
    : public IComponent {
    friend void to_json(nlohmann::json& j, const GameStartStatus& m);
    friend void from_json(const nlohmann::json& j, GameStartStatus& m);

public:
    enum class RenditionStep {
        WAIT,
        APEARPURPOSE,
        WAITAFTERAPERAR,
        CLOSE,
        WAITAFTERCLOSE,
        READY,
        GO,
        WAITAFTERGO,
        GOBACK,
        END,
    };

private: // variables
    bool isAlive_ = true;
    bool isStart_=false;

    float firstWaitTime_;
    float waitTimeAfterApear_;
    float waitTimeAfterClose_;
    float waitTimeAfterGo_;

    ///* pos
    Vec2f purposeStartPos_;
    Vec2f purposeEndPos_;
    Vec2f purposeClosePos_;
    Vec2f purposePos_;
    //[^
    Vec2f readyStartPos_;
    Vec2f readyEndPos_;
    Vec2f readyPos_;

    // scale
    Vec2f goStartScale_;
    Vec2f goEndScale_;
    Vec2f goScale_;
    float goRotateSpeed_;
    float goRotate_;

    // ease
    Easing apearEasing_;
    Easing closeEasing_;
    Easing goEasing_;
    Easing readyEasing_;
    Easing goBackEasing_;

    RenditionStep renditionStep_;

  
public:
    GameStartStatus() {}
    virtual ~GameStartStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

    void ApearEasing(const float& deltaTime);
    void CloseEasing(const float& deltaTime);
    void GoEasing(const float& deltaTime);
    void ReadyEasing(const float& deltaTime);
    void GoBackEasing(const float& deltaTime);

    void Reset();

public: // accsessor
    /// getter
    bool GetIsAlive() const { return isAlive_; }
    bool GetIsStart() const { return isStart_; }
    RenditionStep GetRenditionStep() const { return renditionStep_; }
    float GetFirstWaitTime() const { return firstWaitTime_; }
    float GetWaitTimeAfterApear() const { return waitTimeAfterApear_; }
    float GetWaitTimeAfterClose() const { return waitTimeAfterClose_; }
    float GetWaitTimeAfterGo() const { return waitTimeAfterGo_;}
    Vec2f GetGoScale() const { return goScale_; }
    Vec2f GetReadyPos() const { return readyPos_; }
    Vec2f GetPurposePos() const { return purposePos_; }
    float GoRotate() const { return goRotate_; }
  
    //setter
    void SetRenditionStep(const RenditionStep& step) { renditionStep_=step; }
    void SetIsStart(const bool& is) { isStart_ = is; }
};
