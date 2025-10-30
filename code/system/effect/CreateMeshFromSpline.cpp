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
    auto planeRendererComp = getComponent<PlaneRenderer>(_entity);
    if (planeRendererComp == nullptr) {
        return;
    }

    auto splinePointsComp = getComponent<SplinePoints>(_entity);
    if (splinePointsComp == nullptr || splinePointsComp->points_.size() < 4) {
        planeRendererComp->setIsRender(false);
        return;
    }
    planeRendererComp->setIsRender(true);

    auto splinePoints = CatmullRomSpline(splinePointsComp->points_, splinePointsComp->segmentDivide_);

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
        const Vec3f& p0 = splinePointsComp->points_[i];
        const Vec3f& p1 = splinePointsComp->points_[i + 1];

        Vec3f dir = p1 - p0;

        prevTotalLength = totalLength;
        totalLength += dir.length();

        dir = dir.normalize();

        Vec3f up    = axisY;
        Vec3f right = dir.cross(up).normalize();

        Vec2f minUV = Vec2f(splinePointsComp->startUv_[X], std::lerp(splinePointsComp->startUv_[Y], splinePointsComp->endUv_[Y], prevTotalLength / allLength));
        Vec2f maxUV = Vec2f(splinePointsComp->endUv_[X], std::lerp(splinePointsComp->startUv_[Y], splinePointsComp->endUv_[Y], totalLength / allLength));

        Vec2f uv[4];
        uv[0] = minUV;
        uv[1] = Vec2f(maxUV[0], minUV[1]);
        uv[2] = Vec2f(minUV[0], maxUV[1]);
        uv[3] = maxUV;

        // === 縦メッシュ ===
        {
            TextureVertexData vertData;

            Vec3f left0  = p0 - right * (splinePointsComp->width_ * 0.5f);
            Vec3f right0 = p0 + right * (splinePointsComp->width_ * 0.5f);
            Vec3f left1  = p1 - right * (splinePointsComp->width_ * 0.5f);
            Vec3f right1 = p1 + right * (splinePointsComp->width_ * 0.5f);

            // 最初のセグメントは4頂点、それ以降は2頂点だけ追加
            if (i == 0) {
                TextureVertexData v0{Vec4f(left0, 1.0f), uv[0], up};
                TextureVertexData v1{Vec4f(right0, 1.0f), uv[1], up};
                verticalVertexes.push_back(v0);
                verticalVertexes.push_back(v1);
            }

            TextureVertexData v2{Vec4f(left1, 1.0f), uv[2], up};
            TextureVertexData v3{Vec4f(right1, 1.0f), uv[3], up};
            verticalVertexes.push_back(v2);
            verticalVertexes.push_back(v3);

            uint32_t base = static_cast<uint32_t>(verticalVertexes.size() - 4);
            indices.push_back(base + 0);
            indices.push_back(base + 2);
            indices.push_back(base + 1);
            indices.push_back(base + 1);
            indices.push_back(base + 2);
            indices.push_back(base + 3);
        }

        // === 横メッシュ ===
        {
            TextureVertexData vertData;

            Vec3f down0 = p0 - up * (splinePointsComp->width_ * 0.5f);
            Vec3f up0   = p0 + up * (splinePointsComp->width_ * 0.5f);
            Vec3f down1 = p1 - up * (splinePointsComp->width_ * 0.5f);
            Vec3f up1   = p1 + up * (splinePointsComp->width_ * 0.5f);

            if (i == 0) {
                TextureVertexData v0{Vec4f(down0, 1.0f), uv[0], right};
                TextureVertexData v1{Vec4f(up0, 1.0f), uv[1], right};
                horizontalVertexes.push_back(v0);
                horizontalVertexes.push_back(v1);
            }

            TextureVertexData v2{Vec4f(down1, 1.0f), uv[2], right};
            TextureVertexData v3{Vec4f(up1, 1.0f), uv[3], right};
            horizontalVertexes.push_back(v2);
            horizontalVertexes.push_back(v3);
        }
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
