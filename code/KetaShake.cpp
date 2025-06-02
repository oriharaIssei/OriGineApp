//#include "KetaShake.h"
//#include"KetaRandom.h"
//
//template<typename float> float Shake(const float& t, const float& strength);
//template<typename Vec2f> Vec2f Shake(const float& t, const float& strength);
//template<typename Vec3f> Vec3f Shake(const float& t, const float& strength);
//
//
//template<typename T> T Shake(const float& t, const float& strength) {
//    float adjustedStrength = t * strength;
//  
//    if constexpr (std::is_same<T, float>::value) {
//		return KetaRandom::Range(-adjustedStrength, adjustedStrength);
//    } else if constexpr (std::is_same<T, Vec2f>::value) {
//        return Vec2f(KetaRandom::Range(-adjustedStrength, adjustedStrength),
//            KetaRandom::Range(-adjustedStrength, adjustedStrength)
//		);
//	} else if constexpr (std::is_same<T, Vec3f>::value) {
//        return Vec3f(KetaRandom::Range(-adjustedStrength, adjustedStrength),
//            KetaRandom::Range(-adjustedStrength, adjustedStrength),
//            KetaRandom::Range(-adjustedStrength, adjustedStrength)
//		);
//	}
//	else {
//		static_assert(false, "shake Can not Cast ");
//	}
//}
