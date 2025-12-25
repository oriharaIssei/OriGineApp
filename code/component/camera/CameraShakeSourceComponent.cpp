#include "CameraShakeSourceComponent.h"

#ifdef _DEBUG
/// engine
#include "scene/Scene.h"

/// ECS
// component
#include "component/transform/CameraTransform.h"

/// GUI
#include "myGui/MyGui.h"
#endif // _DEBUG

/// util
#include "myRandom/MyRandom.h"

using namespace OriGine;

CameraShakeSourceComponent::CameraShakeSourceComponent() : IComponent() {}
CameraShakeSourceComponent::~CameraShakeSourceComponent() {}

void CameraShakeSourceComponent::Initialize(Scene* /*_scene*/, EntityHandle /*_owner*/) {
    if (isActive) {
        StartShake();
    }
}
void CameraShakeSourceComponent::Finalize() {}

void CameraShakeSourceComponent::StartShake() {
    isActive = true;
    for (size_t i = 0; i < 2; ++i) {
        fragCoord[i] = MyRandom::Float(0.f, resolution[i]).Get();
    }

    elapsedTime = 0.0f;
}

void CameraShakeSourceComponent::StopShake() {
    isActive = false;
}

void CameraShakeSourceComponent::Edit([[maybe_unused]] OriGine::Scene* _scene, [[maybe_unused]] EntityHandle _owner, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG

    std::string label = "";

    // シェイクの種類
    {
        const char* items[] = {"SinCurve", "Noise"};
        int currentItem     = static_cast<int>(type);

        ImGui::Text("Shake Type :");
        label = "##ShakeType" + _parentLabel;
        if (ImGui::BeginCombo(label.c_str(), items[currentItem])) {
            bool isSelected        = false;
            int32_t shakeTypeIndex = 0;
            for (auto* shakeTypeName : items) {
                isSelected = shakeTypeName == items[currentItem];

                if (ImGui::Selectable(shakeTypeName, isSelected)) {
                    OriGine::EditorController::GetInstance()->PushCommand(
                        std::make_unique<SetterCommand<ShakeSourceType>>(&type, static_cast<ShakeSourceType>(shakeTypeIndex)));

                    break;
                }

                ++shakeTypeIndex;
            }
            ImGui::EndCombo();
        }
    }

    auto& cameraTransforms     = _scene->GetComponents<CameraTransform>(_owner);
    int32_t entityMaterialSize = static_cast<int32_t>(cameraTransforms.size());
    // カメラTransformコンポーネントインデックス
    InputGuiCommand<int32_t>("Camera Transform Index##" + _parentLabel, cameraTransformIndex, "%d", [this, entityMaterialSize](int32_t* _newT) {
        this->cameraTransformIndex = std::clamp(*_newT, -1, entityMaterialSize - 1);
    });

    CheckBoxCommand("Is Active##" + _parentLabel, isActive);
    // ループ有無
    CheckBoxCommand("Loop##" + _parentLabel, isLoop);
    // 継続時間
    DragGuiCommand("Duration##" + _parentLabel, duration, 0.01f);
    ImGui::Spacing();

    label = "X##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        // シェイクの強さ
        DragGuiCommand("Amplitude##X" + _parentLabel, axisParameters[X].amplitude, 0.01f);
        // シェイクの速さ
        DragGuiCommand("Frequency##X" + _parentLabel, axisParameters[X].frequency, 0.01f);

        ImGui::TreePop();
    }
    label = "Y##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        // シェイクの強さ
        DragGuiCommand("Amplitude##Y" + _parentLabel, axisParameters[Y].amplitude, 0.01f);
        // シェイクの速さ
        DragGuiCommand("Frequency##Y" + _parentLabel, axisParameters[Y].frequency, 0.01f);

        ImGui::TreePop();
    }
    label = "Z##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        // シェイクの強さ
        DragGuiCommand("Amplitude##Z" + _parentLabel, axisParameters[Z].amplitude, 0.01f);
        // シェイクの速さ
        DragGuiCommand("Frequency##Z" + _parentLabel, axisParameters[Z].frequency, 0.01f);

        ImGui::TreePop();
    }

#endif // _DEBUG
}

void to_json(nlohmann::json& _j, const CameraShakeSourceComponent& _c) {
    _j = nlohmann::json{
        {"isActive", _c.isActive},
        {"type", static_cast<int32_t>(_c.type)},
        {"cameraTransformIndex", _c.cameraTransformIndex},
        {"isLoop", _c.isLoop},
        {"duration", _c.duration},
        {"X_Parameter_Amplitude", _c.axisParameters[X].amplitude},
        {"X_Parameter_Frequency", _c.axisParameters[X].frequency},
        {"Y_Parameter_Amplitude", _c.axisParameters[Y].amplitude},
        {"Y_Parameter_Frequency", _c.axisParameters[Y].frequency},
        {"Z_Parameter_Amplitude", _c.axisParameters[Z].amplitude},
        {"Z_Parameter_Frequency", _c.axisParameters[Z].frequency},
        {"fragCoord", _c.fragCoord},
        {"resolution", _c.resolution}};
}

void from_json(const nlohmann::json& _j, CameraShakeSourceComponent& _c) {
    _j.at("isActive").get_to(_c.isActive);
    int32_t typeInt;
    _j.at("type").get_to(typeInt);
    _c.type = static_cast<ShakeSourceType>(typeInt);
    _j.at("cameraTransformIndex").get_to(_c.cameraTransformIndex);
    _j.at("isLoop").get_to(_c.isLoop);
    _j.at("duration").get_to(_c.duration);
    _j.at("X_Parameter_Amplitude").get_to(_c.axisParameters[X].amplitude);
    _j.at("X_Parameter_Frequency").get_to(_c.axisParameters[X].frequency);
    _j.at("Y_Parameter_Amplitude").get_to(_c.axisParameters[Y].amplitude);
    _j.at("Y_Parameter_Frequency").get_to(_c.axisParameters[Y].frequency);
    _j.at("Z_Parameter_Amplitude").get_to(_c.axisParameters[Z].amplitude);
    _j.at("Z_Parameter_Frequency").get_to(_c.axisParameters[Z].frequency);
    _j.at("fragCoord").get_to(_c.fragCoord);
    _j.at("resolution").get_to(_c.resolution);
}
