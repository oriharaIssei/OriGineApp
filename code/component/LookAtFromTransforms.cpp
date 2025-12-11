#include "LookAtFromTransforms.h"

#ifdef _DEBUG

/// engine
#include "scene/Scene.h"

/// ECS
#include "component/transform/Transform.h"

/// debug
#include "myGui/MyGui.h"
#endif // _DEBUG

LookAtFromTransforms::LookAtFromTransforms() {}
LookAtFromTransforms::~LookAtFromTransforms() {}

void LookAtFromTransforms::Initialize(OriGine::Entity* /*_OriGine::Entity*/) {}
void LookAtFromTransforms::Finalize() {}

void LookAtFromTransforms::Edit([[maybe_unused]] OriGine::Scene* _scene, [[maybe_unused]] OriGine::Entity* _entity, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG

    ImGui::Text("Rotate Axis :");

    std::string label                                           = "X##LookAtFromTransforms" + _parentLabel;
    EnumBitmask<LookAtFromTransforms::RotateAxis> newRotateAxis = rotateAxis;
    bool isChecked                                              = rotateAxis.HasFlag(RotateAxis::X);
    ImGui::Checkbox(label.c_str(), &isChecked);
    if (isChecked) {
        newRotateAxis.SetFlag(RotateAxis::X);
    } else {
        newRotateAxis.ClearFlag(RotateAxis::X);
    }
    label     = "Y##LookAtFromTransforms" + _parentLabel;
    isChecked = rotateAxis.HasFlag(RotateAxis::Y);
    ImGui::Checkbox(label.c_str(), &isChecked);
    if (isChecked) {
        newRotateAxis.SetFlag(RotateAxis::Y);
    } else {
        newRotateAxis.ClearFlag(RotateAxis::Y);
    }
    label     = "Z##LookAtFromTransforms" + _parentLabel;
    isChecked = rotateAxis.HasFlag(RotateAxis::Z);
    ImGui::Checkbox(label.c_str(), &isChecked);
    if (isChecked) {
        newRotateAxis.SetFlag(RotateAxis::Z);
    } else {
        newRotateAxis.ClearFlag(RotateAxis::Z);
    }
    if (rotateAxis != newRotateAxis) {
        auto command = std::make_unique<SetterCommand<EnumBitmask<LookAtFromTransforms::RotateAxis>>>(
            &rotateAxis,
            newRotateAxis);
        OriGine::EditorController::GetInstance()->PushCommand(std::move(command));
    }

    // Transform コンポーネントを持つエンティティ一覧を取得
    auto transformCompArray = _scene->GetComponentArray<OriGine::Transform>();
    auto& transformEntities = transformCompArray->GetEntityIndexBind();
     label       = "FromTransformEntity##" + _parentLabel;
    if (ImGui::BeginCombo(label.c_str(), std::to_string(fromTransformEntity).c_str())) {
        for (auto& entityIndex : transformEntities) {
            int32_t entityId = entityIndex.first;
            bool isSelected  = (fromTransformEntity == entityId);
            if (ImGui::Selectable(std::to_string(entityId).c_str(), isSelected)) {
                auto command = std::make_unique<SetterCommand<int32_t>>(&fromTransformEntity, entityId);
                OriGine::EditorController::GetInstance()->PushCommand(std::move(command));
            }
        }
        ImGui::EndCombo();
    }

    label = "ToTransformEntity##" + _parentLabel;
    if (ImGui::BeginCombo(label.c_str(), std::to_string(toTransformEntity).c_str())) {
        for (auto& entityIndex : transformEntities) {
            int32_t entityId = entityIndex.first;
            bool isSelected  = (toTransformEntity == entityId);
            if (ImGui::Selectable(std::to_string(entityId).c_str(), isSelected)) {
                auto command = std::make_unique<SetterCommand<int32_t>>(&toTransformEntity, entityId);
                OriGine::EditorController::GetInstance()->PushCommand(std::move(command));
            }
        }
        ImGui::EndCombo();
    }

#endif // _DEBUG
}

void to_json(nlohmann::json& _j, const LookAtFromTransforms& _c) {
    _j = nlohmann::json{
        {"rotateAxis", _c.rotateAxis.ToUnderlying()},
        {"fromTransformEntity", _c.fromTransformEntity},
        {"toTransformEntity", _c.toTransformEntity},
    };
}

void from_json(const nlohmann::json& _j, LookAtFromTransforms& _c) {
    _c.rotateAxis          = EnumBitmask<LookAtFromTransforms::RotateAxis>(_j.at("rotateAxis").get<int32_t>());
    _c.fromTransformEntity = _j.at("fromTransformEntity").get<int32_t>();
    _c.toTransformEntity   = _j.at("toTransformEntity").get<int32_t>();
}
