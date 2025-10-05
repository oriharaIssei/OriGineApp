#include "UpdateCameraForward.h"

/// ECS
// component
#include "component/Camera/CameraController.h"
#include "component/Stage/Stage.h"
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

void UpdateCameraForward::UpdateEntity(GameEntity* _entity) {
    CameraController* cameraController = getComponent<CameraController>(_entity);
    CameraTransform* cameraTransform   = getComponent<CameraTransform>(_entity);
    if (cameraController == nullptr || cameraTransform == nullptr) {
        return;
    }
    GameEntity* stageEntity = getUniqueEntity("Stage");
    if (stageEntity == nullptr) {
        return;
    }
    Stage* stage = getComponent<Stage>(stageEntity);

    const Vec3f& cameraPosition                = cameraTransform->translate;
    std::pair<Stage::Link*, Vec3f> nearestLink = {nullptr, Vec3f(0.f, 0.f, 0.f)};

    for (auto& link : stage->getLinksRef()) {
        if (nearestLink.first == nullptr) {
            nearestLink = {
                &link,
                SegmentOnPoint(cameraPosition, stage->getControlPoints()[link.to_].pos_, stage->getControlPoints()[link.from_].pos_)};

            continue;
        }

        Vec3f nearestOnLink = SegmentOnPoint(cameraPosition, stage->getControlPoints()[link.to_].pos_, stage->getControlPoints()[link.from_].pos_);

        if (nearestOnLink.lengthSq() < nearestLink.second.lengthSq()) {
            nearestLink = {
                &link, nearestOnLink};
        }
    }

    if (nearestLink.first == nullptr) {
        return;
    }

    Vec3f forward = stage->getControlPoints()[nearestLink.first->to_].pos_ - stage->getControlPoints()[nearestLink.first->from_].pos_;
    cameraController->setForward(forward.normalize());
}
