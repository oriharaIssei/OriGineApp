#include "StageDebugRender.h"

/// engine
#include "ECSManager.h"

// component
#include "component/Stage.h"

/// math
#include <numbers>

static constexpr uint32_t sphereDivision  = 8;
static constexpr float sphereDivisionReal = static_cast<float>(sphereDivision);

static constexpr float controlPointRadius  = 0.1f; // 制御点の半径
static constexpr uint32_t sphereVertexSize = 4 * sphereDivision * sphereDivision;
static constexpr uint32_t sphereIndexSize  = 4 * sphereDivision * sphereDivision;

static constexpr uint32_t defaultMeshSize = 1000; // メッシュのデフォルトサイズ

static void CreateControlPointMesh(
    Mesh<ColorVertexData>* _mesh,
    const Vec3f& _pos,
    const Vec4f& _color) {

    const float kLatEvery = std::numbers::pi_v<float> / sphereDivisionReal; //* 緯度
    const float kLonEvery = 2.0f * std::numbers::pi_v<float> / sphereDivisionReal; //* 経度

    auto calculatePoint = [&](float lat, float lon) -> Vector3f {
        return {
            _pos[X] + controlPointRadius * std::cos(lat) * std::cos(lon),
            _pos[Y] + controlPointRadius * std::sin(lat),
            _pos[Z] + controlPointRadius * std::cos(lat) * std::sin(lon)};
    };

    // 緯線（緯度方向の円）を描画
    for (uint32_t latIndex = 1; latIndex < sphereDivision; ++latIndex) {
        float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;
        for (uint32_t lonIndex = 0; lonIndex < sphereDivision; ++lonIndex) {
            float lonA = lonIndex * kLonEvery;
            float lonB = (lonIndex + 1) % sphereDivision * kLonEvery;

            Vector3f pointA = calculatePoint(lat, lonA);
            Vector3f pointB = calculatePoint(lat, lonB);

            // 頂点バッファにデータを格納
            _mesh->vertexes_.emplace_back(ColorVertexData{Vec4f(pointA, 1.f), _color});
            _mesh->vertexes_.emplace_back(ColorVertexData{Vec4f(pointB, 1.f), _color});

            // インデックスバッファにデータを格納
            _mesh->indexes_.emplace_back((uint32_t)_mesh->indexes_.size());
            _mesh->indexes_.emplace_back((uint32_t)_mesh->indexes_.size());
        }
    }

    // 経線（経度方向の円）を描画
    for (uint32_t lonIndex = 0; lonIndex < sphereDivision; ++lonIndex) {
        float lon = lonIndex * kLonEvery;
        for (uint32_t latIndex = 0; latIndex < sphereDivision; ++latIndex) {
            float latA = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;
            float latB = -std::numbers::pi_v<float> / 2.0f + kLatEvery * (latIndex + 1);

            Vector3f pointA = calculatePoint(latA, lon);
            Vector3f pointB = calculatePoint(latB, lon);

            // 頂点バッファにデータを格納
            _mesh->vertexes_.emplace_back(ColorVertexData{Vec4f(pointA, 1.f), _color});
            _mesh->vertexes_.emplace_back(ColorVertexData{Vec4f(pointB, 1.f), _color});

            // インデックスバッファにデータを格納
            _mesh->indexes_.emplace_back((uint32_t)_mesh->indexes_.size());
            _mesh->indexes_.emplace_back((uint32_t)_mesh->indexes_.size());
        }
    }
}

void StageDebugRender::Initialize() {
    pointRenderer_ = LineRenderer(std::vector<Mesh<ColorVertexData>>());
    pointRenderer_.Initialize(nullptr);
    pointRenderer_.getMeshGroup()->push_back(Mesh<ColorVertexData>());
    pointRenderer_.getMeshGroup()->back().Initialize(defaultMeshSize * sphereVertexSize, defaultMeshSize * sphereIndexSize);
    pointMeshItr_ = pointRenderer_.getMeshGroup()->begin();

    linkRenderer_ = LineRenderer(std::vector<Mesh<ColorVertexData>>());
    linkRenderer_.Initialize(nullptr);
    linkRenderer_.getMeshGroup()->push_back(Mesh<ColorVertexData>());
    linkRenderer_.getMeshGroup()->back().Initialize(defaultMeshSize * 2.f, defaultMeshSize * 2.f);
    linkMeshItr_ = linkRenderer_.getMeshGroup()->begin();

    lineRenderSystem_.Initialize();
}

void StageDebugRender::Update() {
    // クリア
    pointMeshItr_->vertexes_.clear();
    pointMeshItr_->indexes_.clear();
    linkMeshItr_->vertexes_.clear();
    linkMeshItr_->indexes_.clear();

    auto stageArray = ECSManager::getInstance()->getComponentArray<Stage>();
    if (!stageArray) {
        return;
    }
    /// ステージは 1つしかない前提 なので ザルチェック
    for (auto& [stageEntityIndex, stageIndex] : stageArray->getEntityIndexBind()) {
        std::vector<Stage>& stages = stageArray->getAllComponents()[stageIndex][stageIndex];

        Stage* stage = &stages.back();
        // メッシュを作成
        CreateMeshes(stage);
    }
}

void StageDebugRender::Finalize() {}

void StageDebugRender::CreateMeshes(Stage* _stage) {

    // ControlPoint（球）描画
    const float kRadius = 0.1f;
    const Vec4f kColor  = {0, 1, 0, 1}; // 緑
    for (const auto& cp : _stage->getControlPoints()) {
        // Capacityが足りなかったら 新しいMeshを作成する
        if (pointMeshItr_->getIndexCapacity() <= 0) {
            pointMeshItr_->TransferData();
            ++pointMeshItr_;
            auto& meshGroup = pointRenderer_.getMeshGroup();
            if (pointMeshItr_ == meshGroup->end()) {
                pointMeshItr_ = meshGroup->end();
                meshGroup->push_back(Mesh<ColorVertexData>());
                meshGroup->back().Initialize(defaultMeshSize * sphereVertexSize, defaultMeshSize * sphereIndexSize);
            }
        }
        CreateControlPointMesh(pointMeshItr_._Ptr, cp.pos_, kColor);
    }

    // Link（線）描画
    const auto& cps       = _stage->getControlPoints();
    const Vec4f linkColor = {1, 1, 0, 1}; // 黄色
    for (const auto& link : _stage->getLinks()) {
        if (link.from_ < 0 || link.from_ >= cps.size() || link.to_ < 0 || link.to_ >= cps.size()) {
            continue;
        }

        // Capacityが足りなかったら 新しいMeshを作成する
        if (linkMeshItr_->getIndexCapacity() <= 0) {
            linkMeshItr_->TransferData();
            ++linkMeshItr_;
            auto& meshGroup = pointRenderer_.getMeshGroup();
            if (linkMeshItr_ == meshGroup->end()) {
                linkMeshItr_ = meshGroup->end();
                meshGroup->push_back(Mesh<ColorVertexData>());
                meshGroup->back().Initialize(defaultMeshSize * 2, defaultMeshSize * 2);
            }
        }

        const Vec3f& from = cps[link.from_].pos_;
        const Vec3f& to   = cps[link.to_].pos_;

        // 頂点追加
        uint32_t idx0 = (uint32_t)linkMeshItr_->vertexes_.size();
        uint32_t idx1 = idx0 + 1;
        linkMeshItr_->vertexes_.emplace_back(ColorVertexData{Vec4f(from, 1.f), linkColor});
        linkMeshItr_->vertexes_.emplace_back(ColorVertexData{Vec4f(to, 1.f), linkColor});
        linkMeshItr_->indexes_.emplace_back(idx0);
        linkMeshItr_->indexes_.emplace_back(idx1);
    }

    pointMeshItr_->TransferData();
    linkMeshItr_->TransferData();
}

void StageDebugRender::RenderAll() {
    lineRenderSystem_.StartRender();
    auto commandList = lineRenderSystem_.getDxCommand()->getCommandList();

    // ControlPoint（球）
    pointRenderer_.getTransformBuff().SetForRootParameter(commandList, 0);
    for (auto& mesh : *pointRenderer_.getMeshGroup()) {
        if (mesh.indexes_.empty())
            continue;
        commandList->IASetVertexBuffers(0, 1, &mesh.getVertexBufferView());
        commandList->IASetIndexBuffer(&mesh.getIndexBufferView());
        commandList->DrawIndexedInstanced((UINT)mesh.indexes_.size(), 1, 0, 0, 0);
    }

    // Link（線）
    linkRenderer_.getTransformBuff().SetForRootParameter(commandList, 0);
    for (auto& mesh : *linkRenderer_.getMeshGroup()) {
        if (mesh.indexes_.empty())
            continue;
        commandList->IASetVertexBuffers(0, 1, &mesh.getVertexBufferView());
        commandList->IASetIndexBuffer(&mesh.getIndexBufferView());
        commandList->DrawIndexedInstanced((UINT)mesh.indexes_.size(), 1, 0, 0, 0);
    }
}
