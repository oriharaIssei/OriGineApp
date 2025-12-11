#pragma once
#include "system/ISystem.h"

#include "component/renderer/primitive/PlaneRenderer.h"

struct SplinePoints;

/// <summary>
/// スプラインからメッシュを生成するシステム
/// </summary>
class CreateMeshFromSpline
    : public OriGine::ISystem {
public:
    CreateMeshFromSpline();
    ~CreateMeshFromSpline() override;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(OriGine::Entity* _entity) override;

    void CreateCrossPlaneMesh(OriGine::PlaneRenderer* _planeRendererComp, SplinePoints* _splinePointsComp);
    void CreateLinePlaneMesh(OriGine::PlaneRenderer* _planeRendererComp, SplinePoints* _splinePointsComp);
};
