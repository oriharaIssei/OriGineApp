#include "PillerSpawner.h"
/// externals
#include "imgui/imgui.h"
#include <string>

void FloorAndPillerSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {
    /// 初期化でパラメータ編集してるから大丈夫、ここ消したら終わり
    floorSpace_        = 11.0f;
    pillerSpace_       = 10.0f;
    columNumMax_       = 3;
    firstPillerOffset_ = 5.5f;
    isCreated_         = false;
    safeZoneCostMax_   = 2;
    normalCostMax_     = 6;
    HPMax_             = 2;
    collisionSize_     = 5.0f;
}

bool FloorAndPillerSpawner::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("floorSpace", &floorSpace_, 0.01f);
    isChange |= ImGui::DragFloat("pillerSpace", &pillerSpace_, 0.01f);
    isChange |= ImGui::DragFloat("firstPillerOffset", &firstPillerOffset_, 0.01f);
    isChange |= ImGui::InputInt("columNumMax", &columNumMax_);
    isChange |= ImGui::InputInt("pillarHP", &HPMax_);
    isChange |= ImGui::DragFloat("collisionSize", &collisionSize_, 0.01f);

    return isChange;
}

void FloorAndPillerSpawner::Save(BinaryWriter& _writer) {
    _writer.Write("pillerSpace", pillerSpace_);
    _writer.Write("floorSpace", floorSpace_);
    _writer.Write("columNumMax", columNumMax_);
    _writer.Write("firstPillerOffset", firstPillerOffset_);
    _writer.Write("pillarHP", HPMax_);
    _writer.Write("collisionSize", collisionSize_);
}

void FloorAndPillerSpawner::Load(BinaryReader& _reader) {
    _reader.Read("pillerSpace", pillerSpace_);
    _reader.Read("floorSpace", floorSpace_);
    _reader.Read("columNumMax", columNumMax_);
    _reader.Read("firstPillerOffset", firstPillerOffset_);
    _reader.Read("pillarHP", HPMax_);
    _reader.Read("collisionSize", collisionSize_);
}

void FloorAndPillerSpawner::Finalize() {}
