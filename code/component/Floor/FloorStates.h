#pragma once

#include "component/IComponent.h"

class FloorStates
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    bool isFalling_=false;   // 落下中かどうか
    float fallTimer_;         // 落ちるまでの猶予時間
    bool haveSafeZone_; // 無敵床を持っているかどうか
    bool isDestroy_ = false;

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
    bool GetIsDestory()const { return isDestroy_; }

    bool* GetIsFallingPtr() { return &isFalling_; }
    bool GetIsFalling() const { return isFalling_; }
    /// setter
};
