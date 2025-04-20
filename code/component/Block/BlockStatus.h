#pragma once

#include "component/IComponent.h"
#include"component/Block/BlockManager.h"
#include<array>
#include <cstdint>
#include<Vector3.h>



class BlockStatus
    : public IComponent {
public:
    

private: // variables

    bool isAlive_ = true;
    bool isFall_ = false;
   
    //
    int32_t columNum_;
    int32_t currentHP_;
    bool isDestroy_ = false;


    BlockType blockType_;

public:
    BlockStatus() {}
    virtual ~BlockStatus() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Save(BinaryWriter& _writer) override;
    void Load(BinaryReader& _reader) override;
    void Finalize() override;

    void TakeDamage();
   

public: // accsessor

    /// getter
    int32_t GetColumNum() const { return columNum_; }
    int32_t GetCurrentHP() const { return currentHP_; }
    bool GetIsDestroy() const { return isDestroy_; }
    const bool& GetIsFall() const { return isFall_; }
    BlockType GetBlockType() const { return blockType_; }

      /// setter
    void SetColum(const int32_t& colum);
    void SetIsFall(const bool& is) { isFall_ = is; }
    void SetcurrentHP(const int32_t& hp) { currentHP_ = hp; }
    void SetIsDestroy(const bool& is) { isDestroy_ = is;}
    void SetBlockType(const BlockType& type) { blockType_ = type; }
};
