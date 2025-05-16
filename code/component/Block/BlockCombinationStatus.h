#pragma once

#include "component/Block/BlockManager.h"
#include "component/IComponent.h"
#include <cstdint>
#include <vector>

class BlockStatus;
class BlockCombinationStatus
    : public IComponent {
public:
    friend void to_json(nlohmann::json& _json, const BlockCombinationStatus& _block);
    friend void from_json(const nlohmann::json& _json, BlockCombinationStatus& _block);

private: // variables
    bool isAlive_ = true;

    int32_t conbinationMax_;
    std::vector<BlockStatus*> blockStatusArray_;

public:
    BlockCombinationStatus() {}
    virtual ~BlockCombinationStatus() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Finalize() override;

    void AddBlockStatus(BlockStatus* status);

public: // accsessor
    /// getter

    const std::vector<BlockStatus*>& GetBlockStatusArray() const {return blockStatusArray_;}

    /// setter
};
