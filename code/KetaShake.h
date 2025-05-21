#include "KetaRandom.h"
#include "Vector2.h"
#include "Vector3.h"
#include <numbers>

template <typename T>
T Shake(const float& t, const float& strength) {
    // t を [0, 1] に正規化
    float normalizedT      = std::clamp(t, 0.0f, 1.0f); // t が 0 から 1 の範囲に収まるように
    float adjustedStrength = normalizedT * strength;

    if constexpr (std::is_same<T, float>::value) {
        return KetaRandom::Range(-adjustedStrength, adjustedStrength);
    } else if constexpr (std::is_same<T, Vec2f>::value) {
        return Vec2f(
            KetaRandom::Range(-adjustedStrength, adjustedStrength),
            KetaRandom::Range(-adjustedStrength, adjustedStrength));
    } else if constexpr (std::is_same<T, Vec3f>::value) {
        return Vec3f(
            KetaRandom::Range(-adjustedStrength, adjustedStrength),
            KetaRandom::Range(-adjustedStrength, adjustedStrength),
            KetaRandom::Range(-adjustedStrength, adjustedStrength));
    } else {
        static_assert(false, "Shake cannot cast");
    }
}

template <typename T>
T ShakeWave(const float& t, const float& strength) {
    float normalizedT = std::clamp(t, 0.0f, 1.0f); // 0～1の範囲に収める
    float decay       = (1.0f - normalizedT); // 時間とともに減衰
    float frequency   = 10.0f; // 振動の周波数

    if constexpr (std::is_same<T, float>::value) {
        float wave  = std::sin(frequency * t * std::numbers::pi_v<float>);
        float noise = KetaRandom::Range(-1.0f, 1.0f);
        return decay * strength * (wave + 0.2f * noise); // sin波 + ランダム揺れ
    }

    else if constexpr (std::is_same<T, Vec2f>::value) {
        float waveX   = std::sin(frequency * t * std::numbers::pi_v<float>);
        float waveY   = std::cos(frequency * t * std::numbers::pi_v<float>);
        Vec2f noise = {
            KetaRandom::Range(-1.0f, 1.0f),
            KetaRandom::Range(-1.0f, 1.0f)};
        return Vec2f(
            decay * strength * (waveX + 0.2f * noise[X]),
            decay * strength * (waveY + 0.2f * noise[Y]));
    }

    else if constexpr (std::is_same<T, Vec3f>::value) {
        float waveX   = std::sin(frequency * t * std::numbers::pi_v<float>);
        float waveY   = std::cos(frequency * t * std::numbers::pi_v<float>);
        float waveZ   = std::sin(frequency * t * std::numbers::pi_v<float> /**0.5f*/);
        Vec3f noise = {
            KetaRandom::Range(-1.0f, 1.0f),
            KetaRandom::Range(-1.0f, 1.0f),
            KetaRandom::Range(-1.0f, 1.0f)};
        return decay * strength * (Vec3f(waveX, waveY, waveZ) + 0.2f * noise);
    }
}

template float Shake<float>(const float& t, const float& strength);
template Vec2f Shake<Vec2f>(const float& t, const float& strength);
template Vec3f Shake<Vec3f>(const float& t, const float& strength);

// 明示的なテンプレートインスタンス化
template float ShakeWave<float>(const float& t, const float& strength);
template Vec2f ShakeWave<Vec2f>(const float& t, const float& strength);
template Vec3f ShakeWave<Vec3f>(const float& t, const float& strength);
