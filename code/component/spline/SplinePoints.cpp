#include "SplinePoints.h"

#include "ECS/entity/Entity.h"
#include "scene/Scene.h"

#ifdef _DEBUG
#include "myGui/MyGui.h"
#endif // _DEBUG

SplinePoints::SplinePoints() {}

SplinePoints::~SplinePoints() {}

void SplinePoints::Initialize(Entity* /*_entity*/) {}

void SplinePoints::Edit(Scene* /*_scene*/, Entity* /*_entity*/, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG
    CheckBoxCommand("IsCrossMesh##" + _parentLabel, isCrossMesh_);

    ImGui::Spacing();

    InputGuiCommand("Capacity##" + _parentLabel, capacity_);
    if (capacity_ < 4) {
        capacity_ = 4;
    }

    ImGui::Spacing();

    InputGuiCommand("SegmentDivide##" + _parentLabel, segmentDivide_);
    if (segmentDivide_ < 1) {
        segmentDivide_ = 1;
    }
    DragGuiCommand("SegmentLength##" + _parentLabel, segmentLength_, 0.01f, 0.01f);
    if (segmentLength_ < 0.0f) {
        segmentLength_ = 0.0f;
    }

    ImGui::Spacing();

    EasingComboGui("WidthEaseType##" + _parentLabel, widthEaseType_);

    DragGuiCommand("StartWidth##" + _parentLabel, startWidth_, 0.01f);
    if (startWidth_ < 0.0f) {
        startWidth_ = 0.0f;
    }
    DragGuiCommand("EndWidth##" + _parentLabel, endWidth_, 0.01f);
    if (endWidth_ < 0.0f) {
        endWidth_ = 0.0f;
    }

    ImGui::Spacing();

    EasingComboGui("UV EaseType##" + _parentLabel, uvEaseType_);
    DragGuiVectorCommand("StartUv##" + _parentLabel, startUv_);
    DragGuiVectorCommand("EndUv##" + _parentLabel, endUv_);

    ImGui::Spacing();

    DragGuiCommand("FadeOutTime##" + _parentLabel, fadeoutTime_, 0.01f);
#endif // _DEBUG
}

void SplinePoints::Finalize() {}

void SplinePoints::PushPoint(const Vec3f& _point) {
    if (points_.size() >= static_cast<size_t>(capacity_)) {
        points_.pop_front();
    }
    points_.push_back(_point);
}

void to_json(nlohmann::json& j, const SplinePoints& c) {
    j["capacity"]      = c.capacity_;
    j["segmentDivide"] = c.segmentDivide_;
    j["startUv"]       = c.startUv_;
    j["endUv"]         = c.endUv_;
    j["segmentLength"] = c.segmentLength_;
    j["startWidth"]    = c.startWidth_;
    j["endWidth"]      = c.endWidth_;
    j["fadeoutTime"]   = c.fadeoutTime_;
    j["isCrossMesh"]   = c.isCrossMesh_;
    j["widthEaseType"] = static_cast<int>(c.widthEaseType_);
    j["uvEaseType"]    = static_cast<int>(c.uvEaseType_);
}

void from_json(const nlohmann::json& j, SplinePoints& c) {
    j.at("capacity").get_to(c.capacity_);
    j.at("segmentDivide").get_to(c.segmentDivide_);
    j.at("startUv").get_to(c.startUv_);
    j.at("endUv").get_to(c.endUv_);
    j.at("segmentLength").get_to(c.segmentLength_);
    j.at("startWidth").get_to(c.startWidth_);
    j.at("endWidth").get_to(c.endWidth_);
    j.at("fadeoutTime").get_to(c.fadeoutTime_);

    j.at("isCrossMesh").get_to(c.isCrossMesh_);
    {
        int temp = 0;
        j.at("widthEaseType").get_to(temp);
        c.widthEaseType_ = static_cast<EaseType>(temp);
    }
    {
        int temp = 0;
        j.at("uvEaseType").get_to(temp);
        c.uvEaseType_ = static_cast<EaseType>(temp);
    }
}
