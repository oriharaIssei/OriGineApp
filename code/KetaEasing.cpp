
#include "KetaEasing.h"

///std
#include<numbers>
#include <cmath>


float Lerp(const float& start, const float& end, float t) {
    return (1.0f - t) * start + end * t;
}


Vec2f Lerp(const Vec2f& start, const Vec2f& end, float t) {
    Vec2f result;
    result[X] = (1.0f - t) * start[X] + end[X] * t;
    result[Y] = (1.0f - t) * start[Y] + end[Y] * t;
    return result;
}

Vec3f Lerp(const Vec3f& start, const Vec3f& end, float t) {
    Vec3f result;
    result[X] = (1.0f - t) * start[X] + end[X] * t;
    result[Y] = (1.0f - t) * start[Y] + end[Y] * t;
    result[Z] = (1.0f - t) * start[Z] + end[Z] * t;
    return result;
}


float EaseInElasticAmplitude(float t, const float& totaltime, const float& amplitude, const float& period) {

	if (t <= 0.0f) {
		return 0.0f;
	}
	if (t >= totaltime) {
		return 0.0f;
	}
	float s = period / (2.0f * std::numbers::pi_v<float>) * std::asinf(1.0f);
	t /= totaltime;

	return -amplitude * std::powf(2.0f, 10.0f * (t - 1.0f)) * std::sinf((t - 1.0f - s) * (2.0f * std::numbers::pi_v<float>) / period);
}

float EaseOutElasticAmplitude(float t, float totaltime, float amplitude, float period) {
	if (t <= 0.0f)
		return 0.0f;
	if (t >= totaltime)
		return 0.0f;

	float s = period / (2.0f * std::numbers::pi_v<float>) * std::asin(1.0f);
	t /= totaltime;

	return amplitude * std::pow(2.0f, -10.0f * t) * std::sin((t - s) * (2.0f * std::numbers::pi_v<float>) / period);
}

float EaseInOutElasticAmplitude(float t, float totaltime, float amplitude, float period) {
	if (t <= 0.0f)
		return 0.0f;
	if (t >= totaltime)
		return 0.0f;

	float backPoint = 0.5f;
	t /= totaltime;

	if (t < backPoint) {
		return EaseOutElasticAmplitude(t, totaltime, amplitude, period);
	} else {

		return EaseInElasticAmplitude(t - backPoint, totaltime - backPoint, amplitude, period);
	}
}

template<typename T> T EaseAmplitudeScale(const T& initScale, const float& easeT, const float& totalTime, const float& amplitude, const float& period) {
	T newScale = initScale; // T型のnewScaleを宣言

	if constexpr (std::is_same<T, float>::value) {
		newScale = initScale + -EaseOutElasticAmplitude(easeT, totalTime, amplitude, period);
	} else if constexpr (std::is_same<T, Vec2f>::value) {
		newScale[X] = initScale[X] + -EaseOutElasticAmplitude(easeT, totalTime, amplitude, period);
		newScale[Y] = initScale[Y] + EaseOutElasticAmplitude(easeT, totalTime, amplitude, period);
	} else if constexpr (std::is_same<T, Vec3f>::value) {
		newScale[X] = initScale[X] + -EaseOutElasticAmplitude(easeT, totalTime, amplitude, period);
		newScale[Y] = initScale[Y] + EaseOutElasticAmplitude(easeT, totalTime, amplitude, period);
		newScale[Z] = initScale[Z] + -EaseOutElasticAmplitude(easeT, totalTime, amplitude, period);
	}

	return newScale;
}

// EaseInSine 関数
template<typename T> T EaseInSine(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = 1.0f - std::cosf((t * std::numbers::pi_v<float>) / 2.0f);
	return Lerp(start, end, easeT);
}

// EaseOutSine 関数
template<typename T> T EaseOutSine(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = std::sinf((t * std::numbers::pi_v<float>) / 2.0f);
	return Lerp(start, end, easeT);
}

// EaseInOutSine 関数
template<typename T> T EaseInOutSine(const T& start, const T& end, float x, float totalX) {
	float t = x / (totalX / 2.0f);
	float easeT = 0.5f * (1.0f - std::cosf(t * std::numbers::pi_v<float>));
	return Lerp(start, end, easeT);
}

// EaseInQuint 関数
template<typename T> T EaseInQuint(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = t * t * t * t * t;
	return Lerp(start, end, easeT);
}

// EaseOutQuint 関数
template<typename T> T EaseOutQuint(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = 1.0f - std::powf(1.0f - t, 5);
	return Lerp(start, end, easeT);
}

// EaseInOutQuint 関数
template<typename T> T EaseInOutQuint(const T& start, const T& end, float x, float totalX) {
	float t = x / (totalX / 2.0f);
	float easeT;
	if (t < 1.0f) {
		easeT = 0.5f * t * t * t * t * t;
	} else {
		t -= 2.0f;
		easeT = 0.5f * (std::powf(t, 5) + 2.0f);
	}
	return Lerp(start, end, easeT);
}

// EaseInCirc 関数
template<typename T> T EaseInCirc(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = 1.0f - std::sqrtf(1.0f - std::powf(t, 2));
	return Lerp(start, end, easeT);
}

// EaseOutCirc 関数
template<typename T> T EaseOutCirc(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = std::sqrtf(1.0f - std::powf(t - 1.0f, 2));
	return Lerp(start, end, easeT);
}

// EaseInOutCirc 関数
template<typename T> T EaseInOutCirc(const T& start, const T& end, float x, float totalX) {
	float t = x / (totalX / 2.0f);
	float easeT;
	if (t < 1.0f) {
		easeT = -0.5f * (std::sqrtf(1.0f - std::powf(t, 2)) - 1.0f);
	} else {
		t -= 2.0f;
		easeT = 0.5f * (std::sqrtf(1.0f - std::powf(t, 2)) + 1.0f);
	}
	return Lerp(start, end, easeT);
}

// EaseInExpo 関数
template<typename T> T EaseInExpo(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = (t == 0.0f) ? 0.0f : std::powf(2.0f, 10.0f * (t - 1.0f));
	return Lerp(start, end, easeT);
}

// EaseOutExpo 関数
template<typename T> T EaseOutExpo(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = (t == 1.0f) ? 1.0f : 1.0f - std::powf(2.0f, -10.0f * t);
	return Lerp(start, end, easeT);
}

// EaseInOutExpo 関数
template<typename T> T EaseInOutExpo(const T& start, const T& end, float x, float totalX) {
	float t = x / (totalX / 2.0f);
	float easeT;
	if (t < 1.0f) {
		easeT = 0.5f * std::powf(2.0f, 10.0f * (t - 1.0f));
	} else {
		t -= 1.0f;
		easeT = 0.5f * (2.0f - std::powf(2.0f, -10.0f * t));
	}
	return Lerp(start, end, easeT);
}

// EaseInCubic 関数
template<typename T> T EaseInCubic(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = t * t * t;
	return Lerp(start, end, easeT);
}

// EaseOutCubic 関数
template<typename T> T EaseOutCubic(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = 1.0f - std::powf(1.0f - t, 3);
	return Lerp(start, end, easeT);
}

// EaseInOutCubic 関数
template<typename T> T EaseInOutCubic(const T& start, const T& end, float x, float totalX) {
	float t = x / (totalX / 2.0f);
	float easeT;
	if (t < 1.0f) {
		easeT = 0.5f * t * t * t;
	} else {
		t -= 2.0f;
		easeT = 0.5f * (std::powf(t, 3) + 2.0f);
	}
	return Lerp(start, end, easeT);
}

// EaseInQuad 関数
template<typename T> T EaseInQuad(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = t * t;
	return Lerp(start, end, easeT);
}

// EaseOutQuad 関数
template<typename T> T EaseOutQuad(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = 1.0f - (1.0f - t) * (1.0f - t);
	return Lerp(start, end, easeT);
}

// EaseInOutQuad 関数
template<typename T> T EaseInOutQuad(const T& start, const T& end, float x, float totalX) {
	float t = x / (totalX / 2.0f);
	float easeT;
	if (t < 1.0f) {
		easeT = 0.5f * t * t;
	} else {
		t -= 1.0f;
		easeT = -0.5f * (t * (t - 2.0f) - 1.0f);
	}
	return Lerp(start, end, easeT);
}

// EaseInQuart 関数
template<typename T> T EaseInQuart(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = t * t * t * t;
	return Lerp(start, end, easeT);
}

// EaseOutQuart 関数
template<typename T> T EaseOutQuart(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = 1.0f - std::powf(1.0f - t, 4);
	return Lerp(start, end, easeT);
}

// EaseInOutQuart 関数
template<typename T> T EaseInOutQuart(const T& start, const T& end, float x, float totalX) {
	float t = x / (totalX / 2.0f);
	float easeT;
	if (t < 1.0f) {
		easeT = 0.5f * t * t * t * t;
	} else {
		t -= 2.0f;
		easeT = -0.5f * (std::powf(t, 4) - 2.0f);
	}
	return Lerp(start, end, easeT);
}

/// EaseInBack
template<typename T>
T EaseInBack(const T& start, const T& end, float x, float totalX) {
	const float s = 1.70158f;
	float t = x / totalX;
	float easeT = t * t * ((s + 1) * t - s);
	return Lerp(start, end, easeT);
}

/// EaseOutBack
template<typename T>
T EaseOutBack(const T& start, const T& end, float x, float totalX) {
	const float s = 1.70158f;
	float t = x / totalX - 1;
	float easeT = (t * t * ((s + 1) * t + s)) + 1;
	return Lerp(start, end, easeT);
}


/// EaseInOutBack
template<typename T>
T EaseInOutBack(const T& start, const T& end, float x, float totalX) {
	const float s = 1.70158f * 1.525f;
	float t = x / (totalX / 2.0f);
	float easeT;

	if (t < 1) {
		easeT = 0.5f * (t * t * ((s + 1) * t - s));
	} else {
		t -= 2;
		easeT = 0.5f * ((t * t * ((s + 1) * t + s)) + 2);
	}

	return Lerp(start, end, easeT);
}


//バウンス補助関数
float BounceEaseOut(float x) {

	const float n1 = 7.5625f;
	const float d1 = 2.75f;
	float easeT = 0.0f;

	if (x < 1.0f / d1) {
		easeT = n1 * x * x;
	} else if (x < 2.0f / d1) {
		x -= 1.5f / d1;
		easeT = n1 * x * x + 0.75f;
	} else if (x < 2.5f / d1) {
		x -= 2.25f / d1;
		easeT = n1 * x * x + 0.9375f;
	} else {
		x -= 2.625f / d1;
		easeT = n1 * x * x + 0.984375f;
	}
	return easeT;
}

// EaseInBounce 関数
template<typename T> T EaseInBounce(const T& start, const T& end, float x, float totalX) {
	float t = 1.0f - (x / totalX);
	float easeT = 1.0f - BounceEaseOut(t);
	return Lerp(start, end, easeT);
}

// EaseOutBounce 関数
template<typename T> T EaseOutBounce(const T& start, const T& end, float x, float totalX) {
	float t = x / totalX;
	float easeT = BounceEaseOut(t);
	return Lerp(start, end, easeT);
}

// EaseInOutBounce 関数
template<typename T> T EaseInOutBounce(const T& start, const T& end, float x, float totalX) {
	float t = x / (totalX / 2.0f);
	float easeT;
	if (t < 1.0f) {
		easeT = 0.5f * (1.0f - BounceEaseOut(1.0f - t));
	} else {
		t -= 1.0f;
		easeT = 0.5f * BounceEaseOut(t) + 0.5f;
	}
	return Lerp(start, end, easeT);
}
namespace Back {

	template<typename T> T Lerp(const T& start, const T& end, float t)
	{
		return start + (end - start) * t;
	}

	template<typename T> T  InSineZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			return Lerp(start, end, sinf((t / backPoint) * std::numbers::pi_v<float> *0.5f));
		} else
		{
			return Lerp(end, start, 1.0f - cosf(((t - backPoint) / (totaltime - backPoint)) * std::numbers::pi_v<float> *0.5f));
		}
	}

	template<typename T> T OutSineZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			return Lerp(start, end, 1.0f - cosf((t / backPoint) * std::numbers::pi_v<float> *0.5f));
		} else
		{
			return Lerp(end, start, sinf(((t - backPoint) / (totaltime - backPoint)) * std::numbers::pi_v<float> *0.5f));
		}
	}

	template<typename T> T InOutSineZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			return Lerp(start, end, -0.5f * (cosf(std::numbers::pi_v<float> *(t / backPoint)) - 1.0f));
		} else
		{
			return Lerp(end, start, 0.5f * (1.0f - cosf(std::numbers::pi_v<float> *((t - backPoint) / (totaltime - backPoint)))));
		}
	}

	template<typename T> T InQuadZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / backPoint;
			return Lerp(start, end, normalizedTime * normalizedTime);
		} else
		{
			float normalizedTime = (t - backPoint) / (totaltime - backPoint);
			return Lerp(end, start, -normalizedTime * (normalizedTime - 2));
		}
	}

	template<typename T> T OutQuadZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / backPoint;
			return Lerp(start, end, -normalizedTime * (normalizedTime - 2));
		} else
		{
			float normalizedTime = (t - backPoint) / (totaltime - backPoint);
			return Lerp(end, start, normalizedTime * normalizedTime);
		}
	}

	template<typename T> T InOutQuadZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / (backPoint * 0.5f);
			if (normalizedTime < 1) return Lerp(start, end, 0.5f * normalizedTime * normalizedTime);
			normalizedTime--;
			return Lerp(start, end, -0.5f * (normalizedTime * (normalizedTime - 2) - 1));
		} else
		{
			float normalizedTime = (t - backPoint) / ((totaltime - backPoint) * 0.5f);
			if (normalizedTime < 1) return Lerp(end, start, 0.5f * normalizedTime * normalizedTime);
			normalizedTime--;
			return Lerp(end, start, -0.5f * (normalizedTime * (normalizedTime - 2) - 1));
		}
	}

	template<typename T> T InCubicZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / backPoint;
			return Lerp(start, end, normalizedTime * normalizedTime * normalizedTime);
		} else
		{
			float normalizedTime = (t - backPoint) / (totaltime - backPoint);
			normalizedTime--;
			return Lerp(end, start, normalizedTime * normalizedTime * normalizedTime + 1);
		}
	}

	template<typename T> T OutCubicZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / backPoint;
			normalizedTime--;
			return Lerp(start, end, normalizedTime * normalizedTime * normalizedTime + 1);
		} else
		{
			float normalizedTime = (t - backPoint) / (totaltime - backPoint);
			return Lerp(end, start, normalizedTime * normalizedTime * normalizedTime);
		}
	}

	template<typename T> T InOutCubicZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / (backPoint * 0.5f);
			if (normalizedTime < 1) return Lerp(start, end, 0.5f * normalizedTime * normalizedTime * normalizedTime);
			normalizedTime -= 2;
			return Lerp(start, end, 0.5f * (normalizedTime * normalizedTime * normalizedTime + 2));
		} else
		{
			float normalizedTime = (t - backPoint) / ((totaltime - backPoint) * 0.5f);
			if (normalizedTime < 1) return Lerp(end, start, 0.5f * normalizedTime * normalizedTime * normalizedTime);
			normalizedTime -= 2;
			return Lerp(end, start, 0.5f * (normalizedTime * normalizedTime * normalizedTime + 2));
		}
	}

	template<typename T> T InQuartZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / backPoint;
			return Lerp(start, end, normalizedTime * normalizedTime * normalizedTime * normalizedTime);
		} else
		{
			float normalizedTime = (t - backPoint) / (totaltime - backPoint);
			normalizedTime--;
			return Lerp(end, start, -(normalizedTime * normalizedTime * normalizedTime * normalizedTime - 1));
		}
	}

	template<typename T> T OutQuartZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / backPoint;
			normalizedTime--;
			return Lerp(start, end, -(normalizedTime * normalizedTime * normalizedTime * normalizedTime - 1));
		} else
		{
			float normalizedTime = (t - backPoint) / (totaltime - backPoint);
			return Lerp(end, start, normalizedTime * normalizedTime * normalizedTime * normalizedTime);
		}
	}

	template<typename T> T InOutQuartZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / (backPoint * 0.5f);
			if (normalizedTime < 1) return Lerp(start, end, 0.5f * normalizedTime * normalizedTime * normalizedTime * normalizedTime);
			normalizedTime -= 2;
			return Lerp(start, end, -0.5f * (normalizedTime * normalizedTime * normalizedTime * normalizedTime - 2));
		} else
		{
			float normalizedTime = (t - backPoint) / ((totaltime - backPoint) * 0.5f);
			if (normalizedTime < 1) return Lerp(end, start, 0.5f * normalizedTime * normalizedTime * normalizedTime * normalizedTime);
			normalizedTime -= 2;
			return Lerp(end, start, -0.5f * (normalizedTime * normalizedTime * normalizedTime * normalizedTime - 2));
		}
	}

	template<typename T> T InQuintZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / backPoint;
			return Lerp(start, end, normalizedTime * normalizedTime * normalizedTime * normalizedTime * normalizedTime);
		} else
		{
			float normalizedTime = (t - backPoint) / (totaltime - backPoint);
			normalizedTime--;
			return Lerp(end, start, normalizedTime * normalizedTime * normalizedTime * normalizedTime * normalizedTime + 1);
		}
	}

	template<typename T> T OutQuintZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / backPoint;
			normalizedTime--;
			return Lerp(start, end, normalizedTime * normalizedTime * normalizedTime * normalizedTime * normalizedTime + 1);
		} else
		{
			float normalizedTime = (t - backPoint) / (totaltime - backPoint);
			return Lerp(end, start, normalizedTime * normalizedTime * normalizedTime * normalizedTime * normalizedTime);
		}
	}

	template<typename T> T InOutQuintZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / (backPoint * 0.5f);
			if (normalizedTime < 1) return Lerp(start, end, 0.5f * normalizedTime * normalizedTime * normalizedTime * normalizedTime * normalizedTime);
			normalizedTime -= 2;
			return Lerp(start, end, 0.5f * (normalizedTime * normalizedTime * normalizedTime * normalizedTime * normalizedTime + 2));
		} else
		{
			float normalizedTime = (t - backPoint) / ((totaltime - backPoint) * 0.5f);
			if (normalizedTime < 1) return Lerp(end, start, 0.5f * normalizedTime * normalizedTime * normalizedTime * normalizedTime * normalizedTime);
			normalizedTime -= 2;
			return Lerp(end, start, 0.5f * (normalizedTime * normalizedTime * normalizedTime * normalizedTime * normalizedTime + 2));
		}
	}

	template<typename T> T InExpoZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / backPoint;
			return Lerp(start, end, (normalizedTime == 0) ? 0 : powf(2, 10 * (normalizedTime - 1)));
		} else
		{
			float normalizedTime = (t - backPoint) / (totaltime - backPoint);
			return Lerp(end, start, (normalizedTime == 1) ? 1 : (-powf(2, -10 * normalizedTime) + 1));
		}
	}

	template<typename T> T OutExpoZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / backPoint;
			return Lerp(start, end, (normalizedTime == 1) ? 1 : (-powf(2, -10 * normalizedTime) + 1));
		} else
		{
			float normalizedTime = (t - backPoint) / (totaltime - backPoint);
			return Lerp(end, start, (normalizedTime == 0) ? 0 : powf(2, 10 * (normalizedTime - 1)));
		}
	}

	template<typename T> T InOutExpoZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / (backPoint * 0.5f);
			if (normalizedTime == 0) return start;
			if (normalizedTime == 2) return end;
			if (normalizedTime < 1) return Lerp(start, end, 0.5f * powf(2, 10 * (normalizedTime - 1)));
			normalizedTime--;
			return Lerp(start, end, 0.5f * (-powf(2, -10 * normalizedTime) + 2));
		} else
		{
			float normalizedTime = (t - backPoint) / ((totaltime - backPoint) * 0.5f);
			if (normalizedTime == 0) return end;
			if (normalizedTime == 2) return start;
			if (normalizedTime < 1) return Lerp(end, start, 0.5f * powf(2, 10 * (normalizedTime - 1)));
			normalizedTime--;
			return Lerp(end, start, 0.5f * (-powf(2, -10 * normalizedTime) + 2));
		}
	}

	template<typename T> T InCircZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / backPoint;
			return Lerp(start, end, -(sqrtf(1 - normalizedTime * normalizedTime) - 1));
		} else
		{
			float normalizedTime = (t - backPoint) / (totaltime - backPoint);
			normalizedTime--;
			return Lerp(end, start, sqrtf(1 - normalizedTime * normalizedTime));
		}
	}

	template<typename T> T OutCircZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / backPoint;
			normalizedTime--;
			return Lerp(start, end, sqrtf(1 - normalizedTime * normalizedTime));
		} else
		{
			float normalizedTime = (t - backPoint) / (totaltime - backPoint);
			return Lerp(end, start, -(sqrtf(1 - normalizedTime * normalizedTime) - 1));
		}
	}

	template<typename T> T InOutCircZero(const T& start, const T& end, float t, float totaltime, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / (backPoint * 0.5f);
			if (normalizedTime < 1) return Lerp(start, end, -0.5f * (sqrtf(1 - normalizedTime * normalizedTime) - 1));
			normalizedTime -= 2;
			return Lerp(start, end, 0.5f * (sqrtf(1 - normalizedTime * normalizedTime) + 1));
		} else
		{
			float normalizedTime = (t - backPoint) / ((totaltime - backPoint) * 0.5f);
			if (normalizedTime < 1) return Lerp(end, start, -0.5f * (sqrtf(1 - normalizedTime * normalizedTime) - 1));
			normalizedTime -= 2;
			return Lerp(end, start, 0.5f * (sqrtf(1 - normalizedTime * normalizedTime) + 1));
		}
	}

	template<typename T> T InBackZero(const T& start, const T& end, float t, float totaltime, float s, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / backPoint;
			return Lerp(start, end, normalizedTime * normalizedTime * ((s + 1) * normalizedTime - s));
		} else
		{
			float normalizedTime = (t - backPoint) / (totaltime - backPoint);
			normalizedTime--;
			return Lerp(end, start, normalizedTime * normalizedTime * ((s + 1) * normalizedTime + s) + 1);
		}
	}

	template<typename T> T OutBackZero(const T& start, const T& end, float t, float totaltime, float s, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;

		if (t < backPoint)
		{
			float normalizedTime = t / backPoint;
			normalizedTime--;
			return Lerp(start, end, normalizedTime * normalizedTime * ((s + 1) * normalizedTime + s) + 1);
		} else
		{
			float normalizedTime = (t - backPoint) / (totaltime - backPoint);
			return Lerp(end, start, normalizedTime * normalizedTime * ((s + 1) * normalizedTime - s));
		}
	}

	template<typename T> T InOutBackZero(const T& start, const T& end, float t, float totaltime, float s, float backRaito)
	{
		if (t <= 0.0f) return start;
		if (t >= totaltime) return start;

		float backPoint = totaltime * backRaito;
		float s_modified = s * 1.525f;

		if (t < backPoint)
		{
			float normalizedTime = t / (backPoint * 0.5f);
			if (normalizedTime < 1) return Lerp(start, end, 0.5f * (normalizedTime * normalizedTime * ((s_modified + 1) * normalizedTime - s_modified)));
			normalizedTime -= 2;
			return Lerp(start, end, 0.5f * (normalizedTime * normalizedTime * ((s_modified + 1) * normalizedTime + s_modified) + 2));
		} else
		{
			float normalizedTime = (t - backPoint) / ((totaltime - backPoint) * 0.5f);
			if (normalizedTime < 1) return Lerp(end, start, 0.5f * (normalizedTime * normalizedTime * ((s_modified + 1) * normalizedTime - s_modified)));
			normalizedTime -= 2;
			return Lerp(end, start, 0.5f * (normalizedTime * normalizedTime * ((s_modified + 1) * normalizedTime + s_modified) + 2));
		}
	}
}
// ぷにぷに
template Vec3f EaseAmplitudeScale<Vec3f>(const Vec3f& initScale, const float& easeT, const float& easeTime, const float& amplitude, const float& period);
template Vec2f EaseAmplitudeScale<Vec2f>(const Vec2f& initScale, const float& easeT, const float& easeTime, const float& amplitude, const float& period);
template float EaseAmplitudeScale<float>(const float& initScale, const float& easeT, const float& easeTime, const float& amplitude, const float& period);
//*******************************************************************************************************************************************************************
// Sine**************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************

template Vec3f EaseInSine<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseInSine<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseInSine<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseOutSine<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseOutSine<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseOutSine<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseInOutSine<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseInOutSine<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseInOutSine<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseInQuint<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseInQuint<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseInQuint<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseOutQuint<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseOutQuint<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseOutQuint<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseInOutQuint<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseInOutQuint<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseInOutQuint<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseInCirc<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseInCirc<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseInCirc<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseOutCirc<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseOutCirc<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseOutCirc<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseInOutCirc<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseInOutCirc<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseInOutCirc<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseInExpo<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseInExpo<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseInExpo<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseOutExpo<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseOutExpo<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseOutExpo<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseInOutExpo<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseInOutExpo<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseInOutExpo<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseOutCubic<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalx);
template Vec2f EaseOutCubic<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalx);
template float EaseOutCubic<float>(const float& start, const float& end, float x, float totalx);

template Vec3f EaseInCubic<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalx);
template Vec2f EaseInCubic<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalx);
template float EaseInCubic<float>(const float& start, const float& end, float x, float totalx);

template Vec3f EaseInOutCubic<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalx);
template Vec2f EaseInOutCubic<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalx);
template float EaseInOutCubic<float>(const float& start, const float& end, float x, float totalx);

template Vec3f EaseInQuad<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalx);
template Vec2f EaseInQuad<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalx);
template float EaseInQuad<float>(const float& start, const float& end, float x, float totalx);

template Vec3f EaseOutQuad<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalx);
template Vec2f EaseOutQuad<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalx);
template float EaseOutQuad<float>(const float& start, const float& end, float x, float totalx);

template Vec3f EaseInOutQuad<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalx);
template Vec2f EaseInOutQuad<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalx);
template float EaseInOutQuad<float>(const float& start, const float& end, float x, float totalx);

template Vec3f EaseInQuart<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalx);
template Vec2f EaseInQuart<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalx);
template float EaseInQuart<float>(const float& start, const float& end, float x, float totalx);

template Vec3f EaseOutQuart<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalx);
template Vec2f EaseOutQuart<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalx);
template float EaseOutQuart<float>(const float& start, const float& end, float x, float totalx);

// バウンス補助関数
float BounceEaseOut(float x);

template Vec3f EaseInBounce<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseInBounce<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseInBounce<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseOutBounce<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseOutBounce<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseOutBounce<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseInOutBounce<Vec3f>(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseInOutBounce<Vec2f>(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseInOutBounce<float>(const float& start, const float& end, float x, float totalX);

template Vec3f EaseInBack(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseInBack(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseInBack(const float& start, const float& end, float x, float totalX);

template Vec3f EaseOutBack(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseOutBack(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseOutBack(const float& start, const float& end, float x, float totalX);

template Vec3f EaseInOutBack(const Vec3f& start, const Vec3f& end, float x, float totalX);
template Vec2f EaseInOutBack(const Vec2f& start, const Vec2f& end, float x, float totalX);
template float EaseInOutBack(const float& start, const float& end, float x, float totalX);

namespace Back {

	template float InSineZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f InSineZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito);
	template Vec3f InSineZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito);

	template float OutSineZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f OutSineZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f OutSineZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InOutSineZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f InOutSineZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InOutSineZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InQuadZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f InQuadZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InQuadZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float OutQuadZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f OutQuadZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f OutQuadZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InOutQuadZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f InOutQuadZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InOutQuadZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InCubicZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f InCubicZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InCubicZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float OutCubicZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f OutCubicZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f OutCubicZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InOutCubicZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f InOutCubicZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InOutCubicZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InQuartZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f InQuartZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InQuartZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float OutQuartZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f OutQuartZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f OutQuartZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InOutQuartZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f InOutQuartZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InOutQuartZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InQuintZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f InQuintZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InQuintZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float OutQuintZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f OutQuintZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f OutQuintZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InOutQuintZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f InOutQuintZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InOutQuintZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InExpoZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f InExpoZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InExpoZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float OutExpoZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f OutExpoZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f OutExpoZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InOutExpoZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f InOutExpoZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InOutExpoZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InCircZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f InCircZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InCircZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float OutCircZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f OutCircZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f OutCircZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InOutCircZero(const float& start, const float& end, float t, float totaltime, float backRaito);
	template Vec2f InOutCircZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InOutCircZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InBackZero(const float& start, const float& end, float t, float totaltime, float s, float backRaito);
	template Vec2f InBackZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float s, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InBackZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float s, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float OutBackZero(const float& start, const float& end, float t, float totaltime, float s, float backRaito);
	template Vec2f OutBackZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float s, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f OutBackZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float s, float backRaito); //Vec3fに対する特殊化（必要に応じて）

	template float InOutBackZero(const float& start, const float& end, float t, float totaltime, float s, float backRaito);
	template Vec2f InOutBackZero(const Vec2f& start, const Vec2f& end, float t, float totaltime, float s, float backRaito); // Vec2f型に対する特殊化（必要に応じて）
	template Vec3f InOutBackZero(const Vec3f& start, const Vec3f& end, float t, float totaltime, float s, float backRaito); //Vec3fに対する特殊化（必要に応じて）


}
