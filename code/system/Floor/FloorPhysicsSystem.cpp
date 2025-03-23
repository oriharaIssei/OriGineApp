#include "FloorPhysicsSystem.h"

FloorPhysicsSystem::FloorPhysicsSystem() : ISystem(SystemType::Physics) {}
FloorPhysicsSystem::~FloorPhysicsSystem() {}

void FloorPhysicsSystem::Initialize() {
}

void FloorPhysicsSystem::Finalize() {
}

void FloorPhysicsSystem::UpdateEntity(GameEntity* _entity) {
    _entity;

    // 床の耐久値を確認し、壊れたら床を落とす処理を実装
}
