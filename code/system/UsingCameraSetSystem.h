#pragma once
#include "system/ISystem.h"

class UsingCameraSetSystem
    : public ISystem {
public:
    UsingCameraSetSystem():ISystem(SystemType::Movement){}

     void Initialize() override;
     void Finalize() override;

    void eraseDeadEntity() {
        std::erase_if(entities_, [](GameEntity* _entity) {
            return !_entity || !_entity->isAlive();
        });
    }

protected:
     void UpdateEntity([[maybe_unused]] GameEntity* _entity);
};
