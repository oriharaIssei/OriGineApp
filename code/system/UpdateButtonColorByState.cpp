#include "UpdateButtonColorByState.h"

/// Engine
#define ENGINE_ECS
#include "EngineInclude.h"
// component
#include "component/Button.h"

void UpdateButtonColorByState::Initialize() {
}

void UpdateButtonColorByState::Finalize() {
}

static const float interpolate = 0.1f;
void UpdateButtonColorByState::UpdateEntity(GameEntity* _entity) {
    Button* button = getComponent<Button>(_entity);
    if (button == nullptr) {
        return;
    }

    SpriteRenderer* sprite = getComponent<SpriteRenderer>(_entity);
    if (sprite == nullptr) {
        return;
    }

    if (button->isReleased()) {
        sprite->setColor(Lerp<4, float>(sprite->getColor(), button->getReleaseColor(), interpolate));
    } else if (button->isPressed()) {
        sprite->setColor(Lerp<4, float>(sprite->getColor(), button->getPressColor(),interpolate));
    } else if (button->isHovered()) {
        sprite->setColor(Lerp<4,float>(sprite->getColor(),button->getHoverColor(),interpolate));
    } else {
        sprite->setColor(Lerp<4, float>(sprite->getColor(), button->getNormalColor(), interpolate));
    }
}
