// FloorSystem.h
#pragma once
#include "system/ISystem.h"

#include "component/Block/BlockManager.h"
#include <cstdint>
#include <array>
#include <nlohmann/json.hpp>


///====================================================================
// FloorSystem
///====================================================================
class BlockStatus;
class BlockCombinationStatus;
class Emitter;
class FuseCreateSystem : public ISystem {
private:
    bool isInited_;

    BlockManager* blockSpawner_=nullptr;
    BlockCombinationStatus* blockCombinationStatus_ = nullptr;

    std::array<Emitter*, 1> fireEmitter_;
    std::array<nlohmann::json, 1> fireEmitterData_;

    public:
    FuseCreateSystem();
        ~FuseCreateSystem();

    void Initialize() override;
    void Finalize() override;

    void CreateFuse(GameEntity* _entity, BlockStatus*blockStatus);
    void ChangeFuse(GameEntity* _entity, BlockStatus* blockStatus);
   
   
protected:
    void UpdateEntity(GameEntity* _entity) override;
};
