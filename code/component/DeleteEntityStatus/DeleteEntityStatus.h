#pragma once

#include "component/IComponent.h"
#include <cstdint>
#include <Entity.h>
#include <Vector3.h>

class DeleteEntityStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const DeleteEntityStatus& _timerStatus);
    friend void from_json(const nlohmann::json& _json, DeleteEntityStatus& _timerStatus);

private: // variables
    bool isAlive_ = true;

    float deleteTime_;
  
public:
    DeleteEntityStatus() {}
    virtual ~DeleteEntityStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

    void DecrementTimer(const float&time);
  
public: // accsessor
    /// getter

    float GetDeleteTime() const { return deleteTime_; }
  /*  float GetTimer() const { return timer_; }*/

    void SetDeleteTime(const float& time) { deleteTime_ = time; }
};
