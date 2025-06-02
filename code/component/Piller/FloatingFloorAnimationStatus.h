#pragma once

#include "component/IComponent.h"
#include <array>
#include <cstdint>
#include <Vector3.h>
#include <KetaEasing.h>

enum class ReactionStep {
    NONE,
    DAMAGESHAKE,
    CONSTANTSHAKE,
    CHANGEFALLSTATE,
    FALLED,
    END,
};

class FloatingFloorAnimationStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const FloatingFloorAnimationStatus& _block);
    friend void from_json(const nlohmann::json& _json, FloatingFloorAnimationStatus& _block);

private: // variables
    bool isAlive_ = true;

    float shakeValue_;
    float shakeMaxTime_;

    float constantShakeValue_;

    // ease
    float revivalMaxTime_ = 0.0f;   
  
public:
    FloatingFloorAnimationStatus() {}
    virtual ~FloatingFloorAnimationStatus() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Finalize() override;

public: // accsessor
    /// getter
    float GetShakeValue() const { return shakeValue_; }
    float GetShakeMaxTime() const { return shakeMaxTime_; }
    float GetConstantShakeValue() const { return constantShakeValue_; }
    float GetRevivalMaxTime() const { return revivalMaxTime_; } 
    /// setter
  
   
};
