#pragma once
#include "system/ISystem.h"

class PlaneRenderer;
struct SplinePoints;

/// <summary>
/// スプラインからメッシュを生成するシステム
/// </summary>
class CreateMeshFromSpline
    : public ISystem {
public:
    CreateMeshFromSpline();
    ~CreateMeshFromSpline() override;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* _entity) override;

    void CreateCrossPlaneMesh(PlaneRenderer* _planeRendererComp, SplinePoints* _splinePointsComp);
    void CreateLinePlaneMesh(PlaneRenderer* _planeRendererComp, SplinePoints* _splinePointsComp);
};
