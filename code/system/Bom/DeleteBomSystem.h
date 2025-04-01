#pragma once

#include "system/ISystem.h"

class BomStatus;
class ExplotionCollision;

class DeleteBomSystem
    : public ISystem {
public:
    DeleteBomSystem() ;
    ~DeleteBomSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

    
    void AddExplotionEntity(GameEntity* _entity, ExplotionCollision* _bomStates);

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
   
};
