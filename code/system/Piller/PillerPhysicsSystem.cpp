#include "PillerPhysicsSystem.h"

PillerPhysicsSystem::PillerPhysicsSystem() : ISystem(SystemType::Movement) {}
PillerPhysicsSystem::~PillerPhysicsSystem() {}

void PillerPhysicsSystem::Initialize() {

}

void PillerPhysicsSystem::Finalize() {

}


void PillerPhysicsSystem::UpdateEntity(GameEntity* _entity) {
    _entity;

    // 柱の耐久値を確認し、壊れたら床を落とす処理を実装(いらん（イラン（国））)
}
