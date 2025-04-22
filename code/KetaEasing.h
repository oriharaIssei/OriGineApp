#pragma once
#include "Vector2.h"
#include "Vector3.h"

struct Easing {
	float time;
	float maxTime;
	float backRatio;
	float amplitude;
	float period;
};

float Lerp(const float& start, const float& end, float t) {
    return (1.0f - t) * start + end * t;
}

Vec2f Lerp(const Vec2f& start, const Vec2f& end, float t);

Vec3f Lerp(const Vec3f& start, const Vec3f& end, float t);



float EaseInElasticAmplitude(float t, const float& totaltime, const float& amplitude, const float& period);

float EaseOutElasticAmplitude(float t, float totaltime, float amplitude, float period);

float EaseInOutElasticAmplitude(float t, float totaltime, float amplitude, float period);

template<typename T> T EaseAmplitudeScale(const T& initScale, const float& easeT, const float& totalTime, const float& amplitude, const float& period);

//*******************************************************************************************************************************************************************
// Sine**************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************


template<typename T> T EaseInSine(const T& start, const T& end, float x, float totalX);


template<typename T> T EaseOutSine(const T& start, const T& end, float x, float totalX);


template<typename T> T EaseInOutSine(const T& start, const T& end, float x, float totalX);

//*******************************************************************************************************************************************************************
// Quint**************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************


template<typename T> T EaseInQuint(const T& start, const T& end, float x, float totalX);


template<typename T> T EaseOutQuint(const T& start, const T& end, float x, float totalX);


template<typename T> T EaseInOutQuint(const T& start, const T& end, float x, float totalX);

//*******************************************************************************************************************************************************************
// Circ**************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************

template<typename T> T EaseInCirc(const T& start, const T& end, float x, float totalX);

template<typename T> T EaseOutCirc(const T& start, const T& end, float x, float totalX);

template<typename T> T EaseInOutCirc(const T& start, const T& end, float x, float totalX);

//*******************************************************************************************************************************************************************
// Expo**************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************

template<typename T> T EaseInExpo(const T& start, const T& end, float x, float totalX);


template<typename T> T EaseOutExpo(const T& start, const T& end, float x, float totalX);


template<typename T> T EaseInOutExpo(const T& start, const T& end, float x, float totalX);

//*******************************************************************************************************************************************************************
// Cubic**************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************

template<typename T> T EaseOutCubic(const T& start, const T& end, float x, float totalx);


template<typename T> T EaseInCubic(const T& start, const T& end, float x, float totalx);


template<typename T> T EaseInOutCubic(const T& start, const T& end, float x, float totalx);

//*******************************************************************************************************************************************************************
// Quad**************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************

template<typename T> T EaseInQuad(const T& start, const T& end, float x, float totalx);

template<typename T> T EaseOutQuad(const T& start, const T& end, float x, float totalx);


template<typename T> T EaseInOutQuad(const T& start, const T& end, float x, float totalx);

//*******************************************************************************************************************************************************************
// Quart**************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************

template<typename T> T EaseInQuart(const T& start, const T& end, float x, float totalx);


template<typename T> T EaseOutQuart(const T& start, const T& end, float x, float totalx);

//*******************************************************************************************************************************************************************
// back**************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************


template<typename T> T EaseInBack(const T& start, const T& end, float x, float totalX);

template<typename T> T EaseOutBack(const T& start, const T& end, float x, float totalX);

template<typename T> T EaseInOutBack(const T& start, const T& end, float x, float totalX);

//*******************************************************************************************************************************************************************
// Bounce**************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************

// バウンス補助関数
float BounceEaseOut(float x);

template<typename T> T EaseInBounce(const T& start, const T& end, float x, float totalX);

template<typename T> T EaseOutBounce(const T& start, const T& end, float x, float totalX);


template<typename T> T EaseInOutBounce(const T& start, const T& end, float x, float totalX);


namespace Back {

	template<typename T> T  InSineZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T OutSineZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T InOutSineZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T InQuadZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T OutQuadZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T InOutQuadZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T InCubicZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T OutCubicZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T InOutCubicZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T InQuartZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T OutQuartZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T InOutQuartZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T InQuintZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T OutQuintZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T InOutQuintZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T InExpoZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T OutExpoZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T InOutExpoZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T InCircZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T OutCircZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T InOutCircZero(const T& start, const T& end, float t, float totaltime, float backRaito);

	template<typename T> T InBackZero(const T& start, const T& end, float t, float totaltime, float s, float backRaito);

	template<typename T> T OutBackZero(const T& start, const T& end, float t, float totaltime, float s, float backRaito);

	template<typename T> T InOutBackZero(const T& start, const T& end, float t, float totaltime, float s, float backRaito);


}
