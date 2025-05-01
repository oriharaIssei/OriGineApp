#include"EffectByBlockSpawner.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
/// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
#define ENGINE_COMPONENTS
/// externals
#include "imgui/imgui.h"

#include "EffectByBlockUIStatus.h"
#include "system/EffectByBlock/EffectByBlockDeleteSystem.h"
#include "system/EffectByBlock/EffectByBlockDrawSystem.h"

void EffectByBlockSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool EffectByBlockSpawner::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    //// ComboDigit（桁数）のUI選択
    // static const char* digitLabels[] = {
    //     "ONE",
    //     "TWO",
    //     "THREE",
    //     "FOUR",
    //     "FIVE",
    // };
    // int currentIndex                 = static_cast<int>(digit_);
    // if (ImGui::Combo("Timer Digit", &currentIndex, digitLabels, static_cast<int>(UIDigit::COUNT))) {
    //     digit_ = static_cast<UIDigit>(currentIndex);
    // }

    return isChange;
}

void EffectByBlockSpawner::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    /* _writer.Write("timerDigit", static_cast<int32_t>(digit_));*/
}

void EffectByBlockSpawner::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
  
}

 void EffectByBlockSpawner::Finalize() {

  }


void EffectByBlockSpawner::EffectUISpawn(GameEntity* _entity, const float& value, const EffectType& effectType) {
    if (!_entity)
        return;

    Transform* hostTransform = getComponent<Transform>(_entity);
    if (!hostTransform)
        return;

    const float effectValue = value;
    const EffectType type   = effectType;

    int32_t intValue = static_cast<int32_t>(effectValue);
    int32_t absValue = std::abs(intValue);

    ///*　パラメータべた書きゾーン

    int32_t digitCount = (absValue == 0) ? 1 : static_cast<int32_t>(std::log10(absValue)) + 1;

    int32_t currentOffsetIndex = 0; // 数字用のオフセットインデックス

    for (int i = 0; i < digitCount + 2; ++i) {
        GameEntity* uiEntity = CreateEntity<Transform, Rigidbody, ModelMeshRenderer, EffectByBlockUIStatus>(
            "EffectUI", Transform(), Rigidbody(), ModelMeshRenderer(), EffectByBlockUIStatus());

        Transform* trans = getComponent<Transform>(uiEntity);
        trans->scale     = Vec3f(2.0f, 2.0f, 2.0f);

        EffectByBlockUIStatus* status = getComponent<EffectByBlockUIStatus>(uiEntity);
        ModelMeshRenderer* sprite     = getComponent<ModelMeshRenderer>(uiEntity);
        CreateModelMeshRenderer(sprite, uiEntity, kApplicationResourceDirectory + "/Models/Plane", "Plane.obj");

        switch (i) {
        case 0: // アイコン
            status->SetEffectType(type);
            status->SetCurerntIconTexture();
            status->SetDigit(BlockEffectUIDigit::ICON);
            trans->translate = Vec3f(hostTransform->worldMat[3]) + Vec3f(-5.0f, 0.0f, -6.0f); // 固定位置
            break;
        case 1: // 符号
            status->SetEffectType(type);
            status->SetCurerntSignTexture();
            status->SetDigit(BlockEffectUIDigit::SIGN);
            trans->translate = Vec3f(hostTransform->worldMat[3]) + Vec3f(-1.5f, 0.0f, -6.0f); // 固定位置
            break;
        default: // 数字
            status->SetEffectType(type);
            status->SetValue(effectValue);
            status->SetDigit(static_cast<BlockEffectUIDigit>(i));
            int32_t ditinum = status->GetValueForDigit();
            status->SetCurerntNumberTexture(ditinum);

            // 数字のみオフセットを加算
            trans->translate = Vec3f(hostTransform->worldMat[3]) + Vec3f(static_cast<float>((-2.5f * currentOffsetIndex) + 1.5f), 0.0f, -6.0f);
            ++currentOffsetIndex;
            break;
        }

        std::string textureName = status->GetCurrentTextureName();

        if (textureName.empty()) {
            trans->scale = Vec3f(0.0f, 0.0f, 0.0f);
            continue;
        }

        sprite->setTexture(0, kApplicationResourceDirectory + textureName);

        // status
        status->SetLifeTime(1.0f);

        // rigit body
        Rigidbody* rigit = getComponent<Rigidbody>(uiEntity);
        rigit->setVelocity(Vec3f(0.0f, 1.0f, 0.0f));

        ECSManager* ecs = ECSManager::getInstance();
        ecs->getSystem<EffectByBlockDeleteSystem>()->addEntity(uiEntity);
        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(uiEntity);
        ecs->getSystem<EffectByBlockDrawSystem>()->addEntity(uiEntity);

        ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(uiEntity);
    }
}
