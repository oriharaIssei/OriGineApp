#include "UpdateCameraForward.h"

/// ECS
// component
#include "component/Camera/CameraController.h"
#include "component/transform/CameraTransform.h"

static Vec3f SegmentOnPoint(const Vec3f& _point, const Vec3f& _start, const Vec3f& _end) {
    Vec3f segment = _end - _start;
    float t       = segment.dot(_point - _start) / segment.lengthSq();
    if (t < 0.0f) {
        return _start;
    } else if (t > 1.0f) {
        return _end;
    }
    return _start + segment * t;
}

UpdateCameraForward::UpdateCameraForward() : ISystem(SystemCategory::Movement) {}
UpdateCameraForward::~UpdateCameraForward() {}

void UpdateCameraForward::Initialize() {}
void UpdateCameraForward::Finalize() {}

void UpdateCameraForward::UpdateEntity(Entity* /*_entity*/) {}
