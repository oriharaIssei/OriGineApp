#pragma once
#include "component/IComponent.h"

class StageWall
    : public IComponent {
    friend void to_json(nlohmann::json& j, const StageWall& p);
    friend void from_json(const nlohmann::json& j, StageWall& p);

public:
    StageWall();
    ~StageWall() override;

    void Initialize(Entity* _entity) override;
    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
    void Finalize() override;

private:
    int32_t linkIndex_      = -1; // この床のインデックス
    int32_t toPointIndex_   = -1; // この床のリンク先のインデックス
    int32_t fromPointIndex_ = -1; // この床のリンク元のインデックス
    float width_            = 0.5f; // 壁の高さ(幅)
public:
    int32_t GetLinkIndex() const { return linkIndex_; }
    void SetLinkIndex(int32_t _linkIndex) { linkIndex_ = _linkIndex; }
    int32_t GetToPointIndex() const { return toPointIndex_; }
    void SetToPointIndex(int32_t _toPointIndex) { toPointIndex_ = _toPointIndex; }
    int32_t GetFromPointIndex() const { return fromPointIndex_; }
    void SetFromPointIndex(int32_t _fromPointIndex) { fromPointIndex_ = _fromPointIndex; }
    float GetWidth() const { return width_; }
    void SetWidth(float _width) { width_ = _width; }
};
