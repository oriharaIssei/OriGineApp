#pragma once

#include "component/IComponent.h"
#include"KetaEasing.h"

enum class OperateMode {
    LAUNCH,
    EXPLOTION,
};

class OperateUIStatus
    : public IComponent {
    friend void to_json(nlohmann::json& j, const OperateUIStatus& l);
    friend void from_json(const nlohmann::json& j, OperateUIStatus& l);

private: // variables
    bool isAlive_ = true;

     Easing scalingEase_;
    OperateMode operateMode_ = OperateMode::LAUNCH;
     Vec2f startTextureSize_;
     Vec2f maxTextureSize_;
     Vec2f resultTextureSize_ = {0.0f, 0.0f};
     bool isChange_ = false;

public:
    OperateUIStatus() {}
    virtual ~OperateUIStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    void ChangeInit(const OperateMode&operateMOde);
    void ScalingEaseUpdate(const float& t);

public: // accsessor
    /// getter
    Vec2f GetREsultSize() const { return resultTextureSize_; }
    OperateMode GetOperateMode() const { return operateMode_; }
  
    /// setter
    void SetEasingTime(const float& time) { scalingEase_.time = time; }
    void SetIsChange(const bool& is) { isChange_ = is; }
};
