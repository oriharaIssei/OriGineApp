#include "DeleteExplotionCollision.h"

/// ECS
#define ENGINE_ECS
// component
#include"component/Bom/ExplotionCollision.h"
#include "engine/EngineInclude.h"

DeleteExplotionCollision::DeleteExplotionCollision() : ISystem(SystemType::StateTransition) {}

void DeleteExplotionCollision::Initialize() {
}

void DeleteExplotionCollision::Finalize() {
    /*  entities_.clear();*/
}


DeleteExplotionCollision::~DeleteExplotionCollision() {}

void DeleteExplotionCollision::UpdateEntity(GameEntity* _entity) {
    ExplotionCollision* status = getComponent<ExplotionCollision>(_entity);
    if (!status) {
        return;
    }

    //時間減算
  status->TimeDecrement();

    // 時間で消す
    if (status->GetAdaptTime() <= 0.0f) {
     
        DestroyEntity(_entity);                   
    }
}

