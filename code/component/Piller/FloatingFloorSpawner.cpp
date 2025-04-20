#include "FloatingFloorSpawner.h"
/// externals
#include "imgui/imgui.h"
#include <string>

void FloatingFloorSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {
    /// 初期化でパラメータ編集してるから大丈夫、ここ消したら未定義値が出る
    sideSpace_      = 11.0f;
    positionHeight_ = 10.0f;
    columNumMax_    = 1;
    isCreated_      = false;
    HPMax_          = 2;
}

bool FloatingFloorSpawner::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::InputInt("pillarHP", &HPMax_);
    isChange |= ImGui::InputInt("rowNumber", &rowNumber_);
    isChange |= ImGui::DragFloat3("FallCollisionMin", fallCollisionSizeMin_.v, 0.01f);
    isChange |= ImGui::DragFloat3("FallCollisionMax", fallCollisionSizeMax_.v, 0.01f);
    isChange |= ImGui::DragFloat("sideSpace", &sideSpace_, 0.01f);
    isChange |= ImGui::DragFloat("positionHeight", &positionHeight_, 0.01f);
    ImGui::Text("No Edit");
   
    isChange |= ImGui::InputInt("columNumMax", &columNumMax_);
    return isChange;
}

void FloatingFloorSpawner::Save(BinaryWriter& _writer) {
    _writer.Write("pillerSpace", positionHeight_);
    _writer.Write("columNumMax", columNumMax_);
    _writer.Write("pillarHP", HPMax_);
    _writer.Write<3, float>("FallCollisionMin", fallCollisionSizeMin_);
    _writer.Write<3, float>("FallCollisionMax", fallCollisionSizeMax_);
    _writer.Write("sideSpace", sideSpace_);
    _writer.Write("rowNumber", rowNumber_);
   
}

void FloatingFloorSpawner::Load(BinaryReader& _reader) {
    _reader.Read("pillerSpace", positionHeight_);
    _reader.Read("columNumMax", columNumMax_);
    _reader.Read("pillarHP", HPMax_);
    _reader.Read<3, float>("FallCollisionMin", fallCollisionSizeMin_);
    _reader.Read<3, float>("FallCollisionMax", fallCollisionSizeMax_);
    _reader.Read("sideSpace", sideSpace_);
    _reader.Read("rowNumber", rowNumber_);
}

void FloatingFloorSpawner::Finalize() {}


