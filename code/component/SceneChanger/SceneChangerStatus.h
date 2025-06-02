#pragma once

#include "component/IComponent.h"
#include <Entity.h>


class SceneChangerStatus
    : public IComponent {
public:
    friend void to_json(nlohmann::json& j, const SceneChangerStatus& g);
    friend void from_json(const nlohmann::json& j, SceneChangerStatus& g);

private: // variables
    bool isAlive_ = true;

    bool isGotoResult_   = false;
    bool isBackTitle_ = false;
    bool isGotoGame_ = false;

public:
    SceneChangerStatus() {}
    virtual ~SceneChangerStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

public: // accsessor
    /// getter
    bool GetIsGoToResult() const { return isGotoResult_; }
    bool GetIsBackTitle() const { return isBackTitle_; }
    bool GetIsGoToGame() const { return isGotoGame_; }

    void SetIsGoToResult(const bool& is) { isGotoResult_ = is; }
    void SetIsBackTitle(const bool& is) { isBackTitle_ = is; }
    void SetIsGoToGame(const bool& is) { isGotoGame_ = is; }
};
