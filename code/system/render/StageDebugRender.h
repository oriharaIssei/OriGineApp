#pragma once
#include "system/ISystem.h"

/// stl
#include <vector>

/// engine
#include "component/renderer/MeshRenderer.h"
class Stage;

#include "system/render/LineRenderSystem.h"
#include "system/render/TexturedMeshRenderSystem.h"


class StageDebugRender
    : public ISystem {
public:
    StageDebugRender()
        : ISystem(SystemType::Render) {
    }
    ~StageDebugRender() override = default;

    void Initialize() override;
    void Update() override;
    void Finalize() override;

private:
    void CreateMeshes(Stage* _stage);

    void RenderAll();

private:
    LineRenderSystem lineRenderSystem_; // 線を描画するためのシステム
    LineRenderer pointRenderer_; // 球（ControlPoint）用
    LineRenderer linkRenderer_; // 線（Link）用
    std::vector<Mesh<ColorVertexData>>::iterator pointMeshItr_;
    std::vector<Mesh<ColorVertexData>>::iterator linkMeshItr_;
};
