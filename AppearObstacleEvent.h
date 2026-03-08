#pragma once

/// ECS
// component
#include "entity/EntityHandle.h"

/// <summary>
/// エンティティに力を加えるイベント
/// </summary>
struct AppearObstacleEvent {
    AppearObstacleEvent() = default;
    AppearObstacleEvent(const OriGine::EntityHandle& _obstacleHandle)
        : obstacleHandle(_obstacleHandle) {}

    OriGine::EntityHandle obstacleHandle;
};
