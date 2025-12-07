#include "UpdateButtonColorByState.h"

/// Engine
#define ENGINE_ECS
#include "EngineInclude.h"
// component
#include "component/ui/Button.h"

void UpdateButtonColorByState::Initialize() {
}

void UpdateButtonColorByState::Finalize() {
}

static const float interpolate = 0.1f;
void UpdateButtonColorByState::UpdateEntity(Entity* _entity) {
    Button* button = GetComponent<Button>(_entity);
    if (button == nullptr) {
        return;
    }

    SpriteRenderer* sprite = GetComponent<SpriteRenderer>(_entity);
    if (sprite == nullptr) {
        return;
    }

    if (button->IsReleased()) {
        sprite->SetColor(Lerp<4, float>(sprite->GetColor(), button->GetReleaseColor(), interpolate));
    } else if (button->isPressed()) {
        sprite->SetColor(Lerp<4, float>(sprite->GetColor(), button->GetPressColor(),interpolate));
    } else if (button->isHovered()) {
        sprite->SetColor(Lerp<4,float>(sprite->GetColor(),button->GetHoverColor(),interpolate));
    } else {
        sprite->SetColor(Lerp<4, float>(sprite->GetColor(), button->GetNormalColor(), interpolate));
    }
}
