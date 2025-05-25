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
        END,
    };

private: // variables
    bool isAlive_ = true;

    float firstWaitTime_;
    float waitTimeAfterApear_;
    float waitTimeAfterClose_;

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

    // ease
    Easing apearEasing_;
    Easing closeEasing_;
    Easing goEasing_;
    Easing ReadyEasing_;

    RenditionStep renditionStep_;

  
public:
    GameStartStatus() {}
    virtual ~GameStartStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

    void ApearEasing(const float& deltaTime);
    void CloseEasing(const float& deltaTime);
    void 
 

    void Reset();

public: // accsessor
    /// getter
    bool GetIsAlive() const { return isAlive_; }
    RenditionStep GetRenditionStep() const { return renditionStep_; }
};
