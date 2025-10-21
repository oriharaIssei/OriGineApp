#pragma once
#include "component/IComponent.h"

/// stl
#include <unordered_map>

/// engine
#include "input/Input.h"

class ButtonGroup
    : public IComponent {
    friend void to_json(nlohmann::json& j, const ButtonGroup& r);
    friend void from_json(const nlohmann::json& j, ButtonGroup& r);

public:
    ButtonGroup()           = default;
    ~ButtonGroup() override = default;

    void Initialize(Entity* _entity) override;
    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
    void Finalize() override;

private:
    // ボタンのエンティティIDとボタン番号のマップ
    std::unordered_map<int32_t, int32_t> buttonNumbers_;
    int32_t startButtonNumber_   = 0;
    int32_t currentButtonNumber_ = 0;

    // ボタンを選ぶためのキー ボタン
    std::vector<Key> selectAddKeys_{};
    std::vector<PadButton> selectAddPadButtons_{};
    std::vector<Key> selectSubKeys_{};
    std::vector<PadButton> selectSubPadButtons_{};

    // ボタンを決定するためのキー ボタン
    std::vector<Key> decideKeys_{};
    std::vector<PadButton> decidePadButtons_{};

public:
    int32_t getCurrentButtonNumber() const { return currentButtonNumber_; }
    void setCurrentButtonNumber(int32_t _buttonNumber) { currentButtonNumber_ = _buttonNumber; }
    int32_t getStartButtonNumber() const { return startButtonNumber_; }
    void setStartButtonNumber(int32_t _buttonNumber) { startButtonNumber_ = _buttonNumber; }

    std::unordered_map<int32_t, int32_t>& getButtonNumbers() { return buttonNumbers_; }

    int32_t getButtonNumber(int32_t _entityId) const {
        auto it = buttonNumbers_.find(_entityId);
        if (it != buttonNumbers_.end()) {
            return it->second;
        }
        return -1; // 見つからなかった場合のデフォルト値
    }
    int32_t getEntityId(int32_t _buttonNumber) const {
        for (const auto& pair : buttonNumbers_) {
            if (pair.second == _buttonNumber) {
                return pair.first;
            }
        }
        return -1; // 見つからなかった場合のデフォルト値
    }

    const std::vector<Key>& getSelectAddKeys() const { return selectAddKeys_; }
    const std::vector<PadButton>& getSelectAddPadButtons() const { return selectAddPadButtons_; }
    const std::vector<Key>& getSelectSubKeys() const { return selectSubKeys_; }
    const std::vector<PadButton>& getSelectSubPadButtons() { return selectSubPadButtons_; }
    const std::vector<Key>& getDecideKeys() const { return decideKeys_; }
    const std::vector<PadButton>& getDecidePadButtons() const { return decidePadButtons_; }
};
