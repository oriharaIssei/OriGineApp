#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <Entity.h>
#include <Vector4.h>

enum class ScoreUIStep {
    NONE,
    INIT,
    SCOREUP,
    WAIT,
    ADAPT,
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
   
    // score value
    float pulusScoreValue_ = 0.0f;
    float plusScore_       = 0.0f;
    float totalScore_      = 0.0f;
    float scoreMax_        = 99999.0f;
    float scoreUPValue_    = 0.0f;
    float preScore_        = 0.0f;

    // texture size
    Vec2f basePlusScoreTextureSize_;
    Vec2f baseScoreTextureSize_;
    Vec2f baseScoreScale_ = {1.0f, 1.0f};
    Vec2f plusScoreScale_ = {1.0f, 1.0f};

    // easing
    Easing scoreAdaptEasing_;
    Easing upScpreUpEasing_;
    Easing scaleEasing_;
    Easing scalingEasingBaseScore_;

    // color
    Vec4f scorePlusColor_ = {0.9f, 0.9f, 0.0f, 1.0f};
    Vec4f minusPlusColor_ = {0.0f, 0.0f, 0.7f, 1.0f};

    // time
    float waitTime_;

    // step
    ScoreUIStep scoreUIStep_ = ScoreUIStep::NONE;

public:
    ScoreStatus() {}
    virtual ~ScoreStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    void PlusScoreIncrement(const float& value);
    void PlusScorePlusAnimation(const float& value);
    void TimeIncrementAnimation(const float& value);
    void ScoreUPAmplitudeScaling(const float& value);
    void BaseScoreUPAmplitudeScaling(const float& value);

    void ResetEaseTime();
    void ResetPlusScore();

    void ScoreUPChange();

    void SetTotalScoreValue();

public: // accsessor
    /// getter
    float GetCurrentScore() const { return currentScore_; }
    float GetPlusScoreValue() const { return pulusScoreValue_; }
    Vec2f GetBasePlusScoreTextureSize() const { return basePlusScoreTextureSize_; }
    Vec2f GetBaseTextureSize() const { return baseScoreTextureSize_; }
    Vec2f GetPlusScoreScale() const { return plusScoreScale_; }
    Vec2f GetBaseScoreScale() const { return baseScoreScale_; }
    float GetPlusScore() const { return plusScore_; }
    float GetWaitTime() const { return waitTime_; }
    const float GetScoreMax() const { return scoreMax_; }
    bool GetIsScoreChange() const { return isChanging_; }
    ScoreUIStep GetScoreUIStep() const { return scoreUIStep_; }
    Vec4f GetScorePlusColor() const { return scorePlusColor_; }
    Vec4f GetMinusScoreColor() const { return minusPlusColor_; }

    /// setter

    void SetCurrentScore(const float& scoreChange) { currentScore_ = scoreChange; }
    void SetIsChanging(const bool& is) { isChanging_ = is; }
    void SetScoreUIStep(const ScoreUIStep& scoreChange) { scoreUIStep_ = scoreChange; }
};
