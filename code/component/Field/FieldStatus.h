#pragma once

#include "component/IComponent.h"
#include <component/transform/Transform.h>
#include <cstdint>
#include <Entity.h>
#include <Vector3.h>

class FieldStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const FieldStatus& _component);
    friend void from_json(const nlohmann::json& _json, FieldStatus& _component);

private: // variables
    bool isAlive_ = true;
    float fieldLeftMax_;
    float fieldRightMax_;

public:
    FieldStatus() {}
    virtual ~FieldStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    void IncrementCurrentBigBomPoint();
    void ReSetCurrentBigBomPoint();

public: // accsessor
    /// getter
    float GetFieldLeftMax() const { return fieldLeftMax_; }
    float GetFieldRightMax() const { return fieldRightMax_; }

    /// setter
};
