#pragma once

#include "component/IComponent.h"

class PillerStatus;
class FloorStates
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    float fallTimer_; // 落ちるまでの猶予時間
    bool haveSafeZone_; // 無敵床を持っているかどうか
    bool isDestroy_ = false;

    PillerStatus* parentStatus_ = nullptr;

public:
    FloorStates() {}
    virtual ~FloorStates() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

public: // accsessor
    /// getter
    bool* GetIsDestroyPtr() { return &isDestroy_; }
    bool GetIsDestory() const { return isDestroy_; }
    bool GetIsFalling() const;
    PillerStatus* GetParentStatus() {return parentStatus_; }
    /// setter
    void SetParentStatus(PillerStatus* status) { parentStatus_ = status; }
};
