#include "EffectByBlockSpawner.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
/// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
#define ENGINE_COMPONENTS
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

#include "EffectByBlockAnimationStatus.h"
#include "EffectByBlockUIStatus.h"
#include "system/EffectByBlock/EffectByBlockDeleteSystem.h"
#include "system/EffectByBlock/EffectByBlockDrawSystem.h"
#include"system/EffectByBlock/EffectByBlockAnimationSystem.h"

void EffectByBlockSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool EffectByBlockSpawner::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiCommand("rotateSpeed_", rotateSpeed_, 0.01f);
    isChange |= DragGuiCommand("fadeOutTime_", fadeOutTime_, 0.01f);
    isChange |= DragGuiCommand("fadeOutValueX_", fadeOutValueX_, 0.01f);
    ImGui::Spacing();
    isChange |= DragGuiCommand("ApearEasing.maxTime", ApearEasing_.maxTime, 0.01f);
    isChange |= DragGuiCommand("CloseEasing.maxTime", FadeOutEasing_.maxTime, 0.01f);

    isChange |= DragGuiVectorCommand("maxScale", maxScale_, 0.01f);

#endif // _DEBUG

    return isChange;
}

void EffectByBlockSpawner::Finalize() {
}

void to_json(nlohmann::json& _json, const EffectByBlockSpawner& _effectByBlockSpawner) {
    _json["isAlive"]              = _effectByBlockSpawner.isAlive_;
    _json["ApearEasing_.maxTime"] = _effectByBlockSpawner.ApearEasing_.maxTime;
    _json["CloseEasing_.maxTime"] = _effectByBlockSpawner.FadeOutEasing_.maxTime;
    _json["maxScale"]             = _effectByBlockSpawner.maxScale_;
    _json["rotateSpeed_"]         = _effectByBlockSpawner.rotateSpeed_;
    _json["fadeOutTime_"]         = _effectByBlockSpawner.fadeOutTime_;
    _json["fadeOutValueX_"]       = _effectByBlockSpawner.fadeOutValueX_;
    /*_json["timerDigit"] = static_cast<int32_t>(_block.digit_);*/
}

void from_json(const nlohmann::json& _json, EffectByBlockSpawner& _effectByBlockSpawner) {
    _json.at("isAlive").get_to(_effectByBlockSpawner.isAlive_);

    if (auto it = _json.find("ApearEasing_.maxTime"); it != _json.end()) {
        _json.at("ApearEasing_.maxTime").get_to(_effectByBlockSpawner.ApearEasing_.maxTime);
    }

    if (auto it = _json.find("CloseEasing_.maxTime"); it != _json.end()) {
        _json.at("CloseEasing_.maxTime").get_to(_effectByBlockSpawner.FadeOutEasing_.maxTime);
    }

    if (auto it = _json.find("maxScale"); it != _json.end()) {
        _json.at("maxScale").get_to(_effectByBlockSpawner.maxScale_);
    }

    if (auto it = _json.find("rotateSpeed_"); it != _json.end()) {
        _json.at("rotateSpeed_").get_to(_effectByBlockSpawner.rotateSpeed_);
    }

    if (auto it = _json.find("fadeOutTime_"); it != _json.end()) {
        _json.at("fadeOutTime_").get_to(_effectByBlockSpawner.fadeOutTime_);
    }

    if (auto it = _json.find("fadeOutValueX_"); it != _json.end()) {
        _json.at("fadeOutValueX_").get_to(_effectByBlockSpawner.fadeOutValueX_);
    }
}
void EffectByBlockSpawner::EffectUISpawn(const Vec3f& position, const float& value, const EffectType& effectType) {

    const float effectValue = value;
    const EffectType type   = effectType;

    int32_t intValue = static_cast<int32_t>(effectValue);
    int32_t absValue = std::abs(intValue);

    ///*　パラメータべた書きゾーン

    int32_t digitCount = (absValue == 0) ? 1 : static_cast<int32_t>(std::log10(absValue)) + 1;

    for (int i = 0; i < digitCount + 2; ++i) {
        GameEntity* uiEntity = CreateEntity<Transform, Rigidbody, ModelMeshRenderer, EffectByBlockUIStatus, EffectByBlockAnimationStatus>(
            "EffectUI", Transform(), Rigidbody(), ModelMeshRenderer(), EffectByBlockUIStatus(), EffectByBlockAnimationStatus());

        Transform* trans = getComponent<Transform>(uiEntity);
        trans->scale     = Vec3f(0.0f, 0.0f, 0.0f);

        EffectByBlockUIStatus* status = getComponent<EffectByBlockUIStatus>(uiEntity);
        ModelMeshRenderer* sprite     = getComponent<ModelMeshRenderer>(uiEntity);
        CreateModelMeshRenderer(sprite, uiEntity, kApplicationResourceDirectory + "/Models/Plane", "Plane.obj");

        float digitSpacing = 2.5f; // 桁ごとの間隔
        float totalWidth   = digitCount * digitSpacing;
        float leftStartX   = position[X] - totalWidth / 2.0f; // 中央揃え

        switch (i) {
        case 0: // アイコン
            status->SetEffectType(type);
            status->SetCurerntIconTexture();
            status->SetDigit(BlockEffectUIDigit::ICON);
            trans->translate = Vec3f(position) + Vec3f(-5.0f, 0.0f, -6.0f); // 固定位置
            break;
        case 1: // 符号
            status->SetEffectType(type);
            status->SetCurerntSignTexture();
            status->SetDigit(BlockEffectUIDigit::SIGN);
            trans->translate = Vec3f(leftStartX, position[Y], -6.0f);
            // 固定位置
            break;
        default: // 数字
            status->SetEffectType(type);
            status->SetValue(effectValue);
            status->SetDigit(static_cast<BlockEffectUIDigit>(i));
            int32_t ditinum = status->GetValueForDigit();
            status->SetCurerntNumberTexture(ditinum);

            // 数字のみオフセットを加算
            trans->translate = Vec3f(leftStartX + digitSpacing * (digitCount - (i - 2)), position[Y], -6.0f);
            break;
        }

        std::string textureName = status->GetCurrentTextureName();

        if (textureName.empty()) {
            trans->scale = Vec3f(0.0f, 0.0f, 0.0f);
            continue;
        }

        sprite->setTexture(0, kApplicationResourceDirectory + textureName);

        // status
        status->SetLifeTime(7.0f);

        // anime status
        EffectByBlockAnimationStatus* animeStatus = getComponent<EffectByBlockAnimationStatus>(uiEntity);
        animeStatus->SetApearEasing(ApearEasing_);
        animeStatus->SetFadeOutEasing(FadeOutEasing_);
        animeStatus->SetMaxScale(maxScale_); // 2,2,2
        animeStatus->SetRotateSpeed(rotateSpeed_);
        animeStatus->SetFadeOutTime(fadeOutTime_);
        animeStatus->SetFadeOutValueX(fadeOutValueX_); 

        // rigit body
        Rigidbody* rigit = getComponent<Rigidbody>(uiEntity);
        rigit->setVelocity(Vec3f(-7.0f, 1.0f, 0.0f));

        ECSManager* ecs = ECSManager::getInstance();
        ecs->getSystem<EffectByBlockDeleteSystem>()->addEntity(uiEntity);
        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(uiEntity);
        ecs->getSystem<EffectByBlockDrawSystem>()->addEntity(uiEntity);
        ecs->getSystem<EffectByBlockAnimationSystem>()->addEntity(uiEntity);

        ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(uiEntity);
    }
}
