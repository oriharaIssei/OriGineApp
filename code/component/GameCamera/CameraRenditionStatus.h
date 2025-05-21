#pragma once

#include "component/IComponent.h"
#include <array>
#include <cstdint>
#include <Vector3.h>

enum class CameraRenditionStep {
    NONE,
    SHAKING,
    END,
};

class CameraRenditionStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const CameraRenditionStatus& _block);
    friend void from_json(const nlohmann::json& _json, CameraRenditionStatus& _block);

private: // variables
    bool isAlive_ = true;

    // shake value(parm)
    float cameraShakeMaxTime_;
    float cameraShakeValue_;

    // offset
    Vec3f caameraShakeOffset_ = {0.0f, 0.0f, 0.0f};

    bool isShaking_  = false;
    float shakeTime_ = 0.0f;

    CameraRenditionStep renditionStep_;

public:
    CameraRenditionStatus() {}
    virtual ~CameraRenditionStatus() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Finalize() override;

    void CameraShake(const float& deltaTime);
    void ShakeReset();

public: // accsessor
    /// getter
    bool GetIsShaking() const { return isShaking_; }
    Vec3f CameraShakeOffset() const { return caameraShakeOffset_; }
    CameraRenditionStep GetRenditionStep() const { return renditionStep_; }
    /// setter
    void SetIsShaking(const bool& is) { isShaking_ = is; }
    void SetRenditionStep(const CameraRenditionStep& step) { renditionStep_ = step; }
};
