#pragma once

#include "component/IComponent.h"


class FloorAndPillerColum
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    bool isCreated_ = false; // 生成されたかどうか;
    int32_t columnHeight_ = 10; // デフォルトの段数

    /// オフセット追加

public:
    FloorAndPillerColum() {}
    virtual ~FloorAndPillerColum() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Save(BinaryWriter& _writer) override;
    void Load(BinaryReader& _reader) override;
    void Finalize() override;

 

public: // accsessor

    /// getter
    int32_t GetColumnHeight() const { return columnHeight_; }
    bool GetIsCreated() const { return isCreated_; }
      /// setter
    void SetColumnHeight(int32_t height) { columnHeight_ = height; }
  
};
