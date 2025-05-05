#pragma once

#include "component/IComponent.h"
#include <array>
#include <component/transform/Transform.h>
#include <cstdint>
#include <Entity.h>
#include <string>
#include <Vector3.h>

class GameEnd
    : public IComponent {
public:
    friend void to_json(nlohmann::json& j, const GameEnd& g);
    friend void from_json(const nlohmann::json& j, GameEnd& g);

private: // variables
    bool isAlive_ = true;

    bool isGameEnd_   = false;
    bool isBackTitle_ = false;

public:
    GameEnd() {}
    virtual ~GameEnd() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

public: // accsessor
    /// getter
    bool GetIsGameEnd() const { return isGameEnd_; }
    bool GetIsBackTitle() const { return isBackTitle_; }

    void SetIsGameEnd(const bool& is) { isGameEnd_ = is; }
    void SetIsBackTitle(const bool& is) { isBackTitle_ = is; }
};
