#include "BlockBreakEffectSpawner.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
// system
/// externals
#include "ECSManager.h"
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

#include "KetaEasing.h"

void to_json(nlohmann::json& _json, const BlockBreakEffectSpawner& _component) {
    _json["isAlive"]                    = _component.isAlive_;
    _json["frontStartEasing.maxTime"]   = _component.frontStartEasing_.maxTime;
    _json["frontStartEasing.amplitude"] = _component.frontStartEasing_.amplitude;
    _json["frontStartEasing.period"]    = _component.frontStartEasing_.period;
    _json["frontStartEasing.backRatio"] = _component.frontStartEasing_.backRatio;

    _json["backLightEasing.maxTime"]   = _component.backLightEasing_.maxTime;
    _json["backLightEasing.amplitude"] = _component.backLightEasing_.amplitude;
    _json["backLightEasing.period"]    = _component.backLightEasing_.period;
    _json["backLightEasing.backRatio"] = _component.backLightEasing_.backRatio;

    _json["backLightFrameEasing.maxTime"]   = _component.backLightFrameEasing_.maxTime;
    _json["backLightFrameEasing.amplitude"] = _component.backLightFrameEasing_.amplitude;
    _json["backLightFrameEasing.period"]    = _component.backLightFrameEasing_.period;
    _json["backLightFrameEasing.backRatio"] = _component.backLightFrameEasing_.backRatio;

    _json["startRotateSpeed"] = _component.startRotateSpeed_;
    _json["alphaSpeed"]       = _component.alphaSpeed_;
    _json["maxScale"]         = _component.maxScale_;
}

void from_json(const nlohmann::json& _json, BlockBreakEffectSpawner& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    _json.at("frontStartEasing.maxTime").get_to(_component.frontStartEasing_.maxTime);
    _json.at("frontStartEasing.amplitude").get_to(_component.frontStartEasing_.amplitude);
    _json.at("frontStartEasing.period").get_to(_component.frontStartEasing_.period);
    _json.at("frontStartEasing.backRatio").get_to(_component.frontStartEasing_.backRatio);

    _json.at("backLightEasing.maxTime").get_to(_component.backLightEasing_.maxTime);
    _json.at("backLightEasing.amplitude").get_to(_component.backLightEasing_.amplitude);
    _json.at("backLightEasing.period").get_to(_component.backLightEasing_.period);
    _json.at("backLightEasing.backRatio").get_to(_component.backLightEasing_.backRatio);

    _json.at("backLightFrameEasing.maxTime").get_to(_component.backLightFrameEasing_.maxTime);
    _json.at("backLightFrameEasing.amplitude").get_to(_component.backLightFrameEasing_.amplitude);
    _json.at("backLightFrameEasing.period").get_to(_component.backLightFrameEasing_.period);
    _json.at("backLightFrameEasing.backRatio").get_to(_component.backLightFrameEasing_.backRatio);

    _json.at("startRotateSpeed").get_to(_component.startRotateSpeed_);
    _json.at("alphaSpeed").get_to(_component.alphaSpeed_);
    _json.at("maxScale").get_to(_component.maxScale_);
}

void BlockBreakEffectSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {
    // 初期化処理が必要ならここに記述
}

bool BlockBreakEffectSpawner::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange |= ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiCommand("startRotateSpeed", startRotateSpeed_, 0.01f);
    isChange |= DragGuiCommand("alphaSpeed", alphaSpeed_);

    ImGui::Text("frontStartEasing");
    isChange |= DragGuiCommand("frontStartEasing.maxTime", frontStartEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("frontStartEasing.amplitude", frontStartEasing_.amplitude, 0.01f);
    isChange |= DragGuiCommand("frontStartEasing.period", frontStartEasing_.period, 0.01f);
    isChange |= DragGuiCommand("frontStartEasing.backRatio", frontStartEasing_.backRatio, 0.01f);

    ImGui::Text("backLightEasing");
    isChange |= DragGuiCommand("backLightEasing.maxTime", backLightEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("backLightEasing.amplitude", backLightEasing_.amplitude, 0.01f);
    isChange |= DragGuiCommand("backLightEasing.period", backLightEasing_.period, 0.01f);
    isChange |= DragGuiCommand("backLightEasing.backRatio", backLightEasing_.backRatio, 0.01f);

    ImGui::Text("backLightFrameEasing");
    isChange |= DragGuiCommand("backLightFrameEasing.maxTime", backLightFrameEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("backLightFrameEasing.amplitude", backLightFrameEasing_.amplitude, 0.01f);
    isChange |= DragGuiCommand("backLightFrameEasing.period", backLightFrameEasing_.period, 0.01f);
    isChange |= DragGuiCommand("backLightFrameEasing.backRatio", backLightFrameEasing_.backRatio, 0.01f);

    ImGui::Text("maxScale");
    isChange |= DragGuiVectorCommand("maxScale[0]", maxScale_[0]);
    isChange |= DragGuiVectorCommand("maxScale[1]", maxScale_[1]);
    isChange |= DragGuiVectorCommand("maxScale[2]", maxScale_[2]);

#endif // _DEBUG

    return isChange;
}

void BlockBreakEffectSpawner::Finalize() {}

void BlockBreakEffectSpawner::BreakEffectSpawn(const Vec3f& position) {
    for (int32_t i = 0; i < 3; ++i) {

        GameEntity* effectEntity = CreateEntity<Transform, Rigidbody, ModelMeshRenderer>(
            "BreakEffect", Transform(), Rigidbody(), ModelMeshRenderer());

        Transform* transform = getComponent<Transform>(effectEntity);
        transform->translate = position;

        ECSManager* ecs = ECSManager::getInstance();

        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(effectEntity);
        ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(effectEntity);
    }
}

void BlockBreakEffectSpawner::Reset() {
}
