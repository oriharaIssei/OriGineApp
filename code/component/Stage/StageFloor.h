#pragma once
#include "component/IComponent.h"

class StageFloor
    : public IComponent {
    friend void to_json(nlohmann::json& j, const StageFloor& p);
    friend void from_json(const nlohmann::json& j, StageFloor& p);

public:
    StageFloor();
    ~StageFloor() override;

    void Initialize(Entity* _entity) override;
    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
    void Finalize() override;

private:
    int32_t linkIndex_      = -1; // この床のインデックス
    int32_t toPointIndex_   = -1; // この床のリンク先のインデックス
    int32_t fromPointIndex_ = -1; // この床のリンク元のインデックス
    float width_            = 0.5f; // 床の幅
public:
    int32_t getLinkIndex() const { return linkIndex_; }
    void setLinkIndex(int32_t _linkIndex) { linkIndex_ = _linkIndex; }
    int32_t getToPointIndex() const { return toPointIndex_; }
    void setToPointIndex(int32_t _toPointIndex) { toPointIndex_ = _toPointIndex; }
    int32_t getFromPointIndex() const { return fromPointIndex_; }
    void setFromPointIndex(int32_t _fromPointIndex) { fromPointIndex_ = _fromPointIndex; }
    float getWidth() const { return width_; }
    void setWidth(float _width) { width_ = _width; }
};
