#pragma once

#include "system/ISystem.h"

class BomStatus;
class BigExplotionCollision;
class DeleteBigBomSystem
    : public ISystem {
public:
    DeleteBigBomSystem();
    ~DeleteBigBomSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

    
    void AddExplotionEntity(GameEntity* _entity, BigExplotionCollision* _bomStates);

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
   
};
