#include "BulletSpawner.h"

/// externals
#include "imgui/imgui.h"

BulletSpawner::BulletSpawner() {}

void BulletSpawner::Initialize(GameEntity* _hostEntity) {
    _hostEntity;
    isFire_ = false;
}

bool BulletSpawner::Edit() {
    bool isChange = false;

    ImGui::Text("bulletOffset");
    isChange |= ImGui::InputFloat3("##bulletOffset", bulletOffset_.v);

    ImGui::Spacing();

    ImGui::Text("fireCoolTime");
    isChange |= ImGui::InputFloat("##fireCoolTime", &fireCoolTime_);

    ImGui::Text("fireCoolTimeMax");
    isChange |= ImGui::InputFloat("##fireCoolTimeMax", &fireCoolTimeMax_);

    ImGui::Spacing();

    ImGui::Text("bulletSpeed");
    isChange |= ImGui::InputFloat("##bulletSpeed", &bulletSpeed_);
    ImGui::Text("bulletLifeTime");
    isChange |= ImGui::InputFloat("##bulletLifeTime", &bulletLifeTime_);
    ImGui::Text("bulletSize");
    isChange |= ImGui::InputFloat("##bulletSize", &bulletSize_);
    return isChange;
}

void BulletSpawner::Save(BinaryWriter& _writer) {
    _writer.Write<3, float>("bulletOffset", bulletOffset_);
    _writer.Write("fireCoolTime", fireCoolTime_);
    _writer.Write("fireCoolTimeMax", fireCoolTimeMax_);
    _writer.Write("bulletSpeed", bulletSpeed_);
    _writer.Write("bulletLifeTime", bulletLifeTime_);
    _writer.Write("bulletSize", bulletSize_);
}

void BulletSpawner::Load(BinaryReader& _reader) {
    _reader.Read<3, float>("bulletOffset", bulletOffset_);
    _reader.Read("fireCoolTime", fireCoolTime_);
    _reader.Read("fireCoolTimeMax", fireCoolTimeMax_);
    _reader.Read("bulletSpeed", bulletSpeed_);
    _reader.Read("bulletLifeTime", bulletLifeTime_);
    _reader.Read("bulletSize", bulletSize_);
}

void BulletSpawner::Finalize() {}
