#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"

#include <Entity.h>
#include <Vector2.h>

class FloorUIController
    : public IComponent {
    friend void to_json(nlohmann::json& j, const FloorUIController& m);
    friend void from_json(const nlohmann::json& j, FloorUIController& m);

private: // variables
    bool isAlive_ = true;

    // pos

    Vec3f startPos_ = {0.0f, 0.0f, 0.0f};
    Vec3f endPos_   = {0.0f, 0.0f, 0.0f};

    Vec3f startScale_ = {1.0f, 1.0f, 1.0f};

    // scale

    // ease
    Easing moveEasing_;
    Easing openEasing_;
    Easing closeEasing_;

    float rotateSpeed_ = 0.0f;

    //// step
    // ArrowDirection arrowDirection_;

public:
    FloorUIController() {}
    virtual ~FloorUIController() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

public: // accsessor
public: // accessors
    /// getter
    float GetRotateSpeed() const { return rotateSpeed_; }
    bool GetIsAlive() const { return isAlive_; }
    const Vec3f& GetStartPos() const { return startPos_; }
    const Vec3f& GetEndPos() const { return endPos_; }
    const Vec3f& GetStartScale() const { return startScale_; }
 
    const Easing& GetMoveEasing() const { return moveEasing_; }
    const Easing& GetOpenEasing() const { return openEasing_; }
    const Easing& GetCloseEasing() const { return closeEasing_; }

  
};
