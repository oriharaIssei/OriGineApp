#pragma once

#include "component/IComponent.h"
#include<array>
#include <cstdint>
#include<Vector3.h>

class FloorAndPillerrStatus
    : public IComponent {

private: // variables

    bool isAlive_ = true;

    bool isFall_;
    int32_t columNum_;
    int32_t rowNum_;
    float savePosY_;
    float fallValue_;

public:
    FloorAndPillerrStatus() {}
    virtual ~FloorAndPillerrStatus() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Save(BinaryWriter& _writer) override;
    void Load(BinaryReader& _reader) override;
    void Finalize() override;

 

public: // accsessor

    /// getter
    int32_t GetColumNum() const { return columNum_; }
    int32_t GetRowNum() const { return rowNum_; }
    bool GetIsFall() const { return isFall_; }
    float GetSavePos() const { return savePosY_; }

      /// setter
    void SetColumAndRow(const int32_t& colum, const int32_t& row);
    void SetIsFall(const bool& is) { isFall_ = is; }
    void SetSavePos(const float& pos) { savePosY_ = pos; }
};
