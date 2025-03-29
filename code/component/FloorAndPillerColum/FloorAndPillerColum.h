#pragma once

#include "component/IComponent.h"


class FloorAndPillerColum
    : public IComponent {

private: // variables

    bool isAlive_ = true;
    float fallSpeed_;
    int32_t columnHeight_; // デフォルトの段数
    int32_t gridNum_;     // デフォルトのグリッド数

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
      /// setter
    void SetColumnHeight(int32_t height) { columnHeight_ = height; }
  
};
