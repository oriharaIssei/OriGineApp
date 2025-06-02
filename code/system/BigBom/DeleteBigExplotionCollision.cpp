#include "DeleteBigExplotionCollision.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/BigBom/BigExplotionCollision.h"
#include "engine/EngineInclude.h"

DeleteBigExplotionCollision::DeleteBigExplotionCollision() : ISystem(SystemType::StateTransition) {}

void DeleteBigExplotionCollision::Initialize() {
}

void DeleteBigExplotionCollision::Finalize() {
    /*  entities_.clear();*/
}

DeleteBigExplotionCollision::~DeleteBigExplotionCollision() {}

void DeleteBigExplotionCollision::UpdateEntity(GameEntity* _entity) {
    BigExplotionCollision* status = getComponent<BigExplotionCollision>(_entity);
    if (!status) {
        return;
    }

    // 時間減算
    status->TimeDecrement();

    // 時間で消す
    if (status->GetAdaptTime() <= 0.0f) {

        DestroyEntity(_entity);
    }
}
