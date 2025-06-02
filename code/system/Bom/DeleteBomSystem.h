#pragma once

#include "system/ISystem.h"
#include <array>

class BomStatus;
class ExplotionCollision;
class Emitter;
class DeleteBomSystem
    : public ISystem {
public:
    DeleteBomSystem() ;
    ~DeleteBomSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

    
    void AddExplotionEntity(GameEntity* _entity, ExplotionCollision* _bomStates);
    void CreateEffect(GameEntity* _entity);

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;

private:
    std::array<Emitter*, 4> effect_;
    std::array<Emitter*, 4> comEffect_;
};
