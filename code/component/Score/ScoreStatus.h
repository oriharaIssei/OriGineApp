#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <Entity.h>

enum class ScoreUIStep {
    NONE,
    INIT,
    SCOREUP,
    WAIT,
    SCALING, // 揺れも追加
    DOWN,
    ADAPTSCORE,
    END,
};

class ScoreStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const ScoreStatus& _component);
    friend void from_json(const nlohmann::json& _json, ScoreStatus& _component);

private: // variables
    bool isAlive_ = true;

    static float currentScore_;
    bool isChanging_ = false;

    float pulusScoreValue_ = 0.0f;

    float scoreMax_ = 99999.0f;

    Easing scoreAdaptEasing_;
    Easing upScpreUpEasing_;
    Easing scaleEasing_;

    float waitTime_;

    ScoreUIStep scoreUIStep_ = ScoreUIStep::NONE;

public:
    ScoreStatus() {}
    virtual ~ScoreStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    void PlusScoreIncrement(const float& value);
    void TimeIncrementAnimation(const float& value);

    void Reset();

public: // accsessor
    /// getter
    float GetCurrentScore() const { return currentScore_; }
    float GetPlusScoreValue() const { return pulusScoreValue_; }
    const float GetScoreMax() const { return scoreMax_; }
    bool GetIsScoreChange() const { return isChanging_; }
    ScoreUIStep GetScoreUIStep() const { return scoreUIStep_; }

    /// setter

    void SetCurrentScore(const float& scoreChange) { currentScore_ = scoreChange; }
    void SetIsScoreAnimation(const bool& is) { isChanging_ = is; }
    void SetScoreUIStep(const ScoreUIStep& scoreChange) { scoreUIStep_ = scoreChange; }
   
};
