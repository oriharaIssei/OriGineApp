#include "CreateMeshFromSpline.h"

/// component
#include "component/renderer/primitive/PlaneRenderer.h"
#include "component/spline/SplinePoints.h"

/// math
#include <math/Vector3.h>

Vec3f CatmullRomSpline(const Vec3f& _p0, const Vec3f& _p1, const Vec3f& _p2, const Vec3f& _p3, float _t) {
    float t2 = _t * _t;
    float t3 = t2 * _t;

    return 0.5f * ((2.0f * _p1) + (-_p0 + _p2) * _t + (2.0f * _p0 - 5.0f * _p1 + 4.0f * _p2 - _p3) * t2 + (-_p0 + 3.0f * _p1 - 3.0f * _p2 + _p3) * t3);
}

std::deque<Vec3f> CatmullRomSpline(const std::deque<Vec3f>& _points, int _samplePerSegment) {
    std::deque<Vec3f> result;

    /// コントロールポイントが4個以下なら何もできない
    if (_points.size() < 4) {
        return result;
    }

    for (size_t i = 0; i < _points.size() - 3; i++) {
        for (size_t s = 0; s < _samplePerSegment; s++) {
            float t = static_cast<float>(s) / _samplePerSegment;
            result.push_back(CatmullRomSpline(
                _points[i + 0],
                _points[i + 1],
                _points[i + 2],
                _points[i + 3],
                t));
        }
    }

    // 最後のポイントも追加（p2）
    result.push_back(_points[_points.size() - 2]);
    return result;
}

CreateMeshFromSpline::CreateMeshFromSpline() : ISystem(SystemCategory::Effect) {}

CreateMeshFromSpline::~CreateMeshFromSpline() {}

void CreateMeshFromSpline::Initialize() {}

void CreateMeshFromSpline::Finalize() {}

void CreateMeshFromSpline::UpdateEntity(Entity* _entity) {
    auto splinePointsComp = getComponent<SplinePoints>(_entity);
    if (splinePointsComp == nullptr || splinePointsComp->points_.size() < 4) {
        return;
    }
    auto planeRendererComp = getComponent<PlaneRenderer>(_entity);
    if (planeRendererComp == nullptr) {
        return;
    }

    auto splinePoints = CatmullRomSpline(splinePointsComp->points_, splinePointsComp->segmentDivide_);
    if (splinePoints.empty()) {
        return;
    }

    // メッシュ生成
    // 十字型にするため,縦方向と横方向の2つのメッシュを作成
    std::vector<TextureVertexData> verticalVertexes;
    std::vector<TextureVertexData> horizontalVertexes;
    std::vector<uint32_t> indices;

    int32_t segmentCount = static_cast<int32_t>(splinePointsComp->points_.size() - 1);
    // enduv が splineの最長時のメッシュに依存するため,あらかじめ計算しておく
    float allLength       = static_cast<float>(splinePointsComp->segmentLength_ * splinePointsComp->capacity_);
    float totalLength     = 0.0f;
    float prevTotalLength = 0.0f;
    for (int32_t i = 0; i < segmentCount; i++) {
        Vec3f dir = splinePointsComp->points_[i + 1] - splinePointsComp->points_[i];

        prevTotalLength = totalLength;
        totalLength += dir.length();

        dir = dir.normalize();

        float pitch = atan2(dir[Z], dir[Y]);

        Vec3f up    = axisY * MakeMatrix::RotateX(pitch);
        Vec3f right = dir.cross(up).normalize();
        up          = right.cross(dir).normalize();

        Vec2f minUV = Vec2f(0.f, std::lerp(splinePointsComp->startUv_[Y], splinePointsComp->endUv_[Y], prevTotalLength / allLength));
        Vec2f maxUV = Vec2f(1.f, std::lerp(splinePointsComp->startUv_[Y], splinePointsComp->endUv_[Y], totalLength / allLength));

        Vec2f uv[4];
        uv[0] = minUV;
        uv[1] = Vec2f(maxUV[0], minUV[1]);
        uv[2] = Vec2f(minUV[0], maxUV[1]);
        uv[3] = maxUV;

        TextureVertexData vertData;
        // 縦方向メッシュ
        Vec3f vert[4];
        vert[0] = splinePointsComp->points_[i] - right * (splinePointsComp->width_ * 0.5f);
        vert[1] = splinePointsComp->points_[i] + right * (splinePointsComp->width_ * 0.5f);
        vert[2] = splinePointsComp->points_[i + 1] - right * (splinePointsComp->width_ * 0.5f);
        vert[3] = splinePointsComp->points_[i + 1] + right * (splinePointsComp->width_ * 0.5f);
        for (int32_t j = 0; j < 4; j++) {
            vertData.pos      = Vec4f(vert[j][0], vert[j][1], vert[j][2], 1.0f);
            vertData.texCoord = uv[j];
            vertData.normal   = up;
            verticalVertexes.push_back(vertData);
        }

        // 横方向メッシュ
        vert[0] = splinePointsComp->points_[i] - up * (splinePointsComp->width_ * 0.5f);
        vert[1] = splinePointsComp->points_[i] + up * (splinePointsComp->width_ * 0.5f);
        vert[2] = splinePointsComp->points_[i + 1] - up * (splinePointsComp->width_ * 0.5f);
        vert[3] = splinePointsComp->points_[i + 1] + up * (splinePointsComp->width_ * 0.5f);

        for (int32_t j = 0; j < 4; j++) {
            vertData.pos      = Vec4f(vert[j][0], vert[j][1], vert[j][2], 1.0f);
            vertData.texCoord = uv[j];
            vertData.normal   = right;
            horizontalVertexes.push_back(vertData);
        }

        uint32_t baseIndex = static_cast<uint32_t>(i * 4);
        indices.push_back(baseIndex + 0);
        indices.push_back(baseIndex + 2);
        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 2);
        indices.push_back(baseIndex + 3);
    }

    std::vector<TextureMesh> meshGroup;
    TextureMesh verticalMesh;
    verticalMesh.Initialize(static_cast<UINT>(verticalVertexes.size()), static_cast<UINT>(indices.size()));
    verticalMesh.setVertexData(verticalVertexes);
    verticalMesh.setIndexData(indices);
    meshGroup.push_back(verticalMesh);
    TextureMesh horizontalMesh;
    horizontalMesh.Initialize(static_cast<UINT>(horizontalVertexes.size()), static_cast<UINT>(indices.size()));
    horizontalMesh.setVertexData(horizontalVertexes);
    horizontalMesh.setIndexData(indices);
    meshGroup.push_back(horizontalMesh);

    // メッシュ設定
    planeRendererComp->setIsCulling(false);
    planeRendererComp->setMeshGroup(meshGroup);
    planeRendererComp->getMeshGroup()->at(0).TransferData();
    planeRendererComp->getMeshGroup()->at(1).TransferData();
}
