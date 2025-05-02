#include "BlockStatus.h"
#include"component/transform/Transform.h"
/// externals
#include "imgui/imgui.h"
#include <string>

void BlockStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
   
    currentHP_     = 1;
   /* collisionSize_ = 5.0f;*/
}

bool BlockStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();
   
    return isChange;
}

void BlockStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
   
}

void BlockStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
   
}

void BlockStatus::Finalize() {}

void BlockStatus::SetColum(const int32_t& colum) {
   
    columNum_ = colum;
}

void BlockStatus::TakeDamage() {
    currentHP_--;
}

void BlockStatus::MoveUpdate(const float& time,Transform*transform,const float&moveValue) {

    if (!isMove_) {
        return;
    }

    //移動位置
    float movepos = preMovePos_[X] - moveValue;
    moveEase_.time += time;


    /// スケーリングイージング
    transform->translate[X] = (EaseInCirc(preMovePos_[X], movepos, moveEase_.time, moveEase_.maxTime));

    if (moveEase_.time >= moveEase_.maxTime) {
        moveEase_.time    = moveEase_.maxTime;
        transform->translate[X]   = movepos;
        preMovePos_[X] = transform->translate[X];
        isMove_                   = false;
    }
 }


void BlockStatus::TimeInit() {
     moveEase_.time = 0.0f;
  }
