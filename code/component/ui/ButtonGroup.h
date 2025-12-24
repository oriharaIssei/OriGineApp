#pragma once
#include "component/IComponent.h"

/// stl
#include <unordered_map>

/// engine
#include "input/InputManager.h"

/// <summary>
/// 1シーン内で複数のボタンをグループ化し、選択と決定の操作を管理するコンポーネント
/// </summary>
class ButtonGroup
    : public OriGine::IComponent {
    friend void to_json(nlohmann::json& j, const ButtonGroup& r);
    friend void from_json(const nlohmann::json& j, ButtonGroup& r);

public:
    ButtonGroup()           = default;
    ~ButtonGroup() override = default;

    void Initialize(OriGine::Scene* _scene, OriGine::EntityHandle _owner) override;
    void Edit(OriGine::Scene* _scene, OriGine::EntityHandle _owner, const std::string& _parentLabel) override;
    void Finalize() override;

private:
    // ボタンのエンティティIDとボタン番号のマップ
    // キー: ボタン番号, 値: エンティティID
    std::unordered_map<int32_t, OriGine::EntityHandle> buttonNumbers_;
    int32_t startButtonNumber_   = 0;
    int32_t currentButtonNumber_ = 0;

    // ボタンを選ぶためのキー ボタン
    std::vector<OriGine::Key> selectAddKeys_{};
    std::vector<OriGine::PadButton> selectAddPadButtons_{};
    std::vector<OriGine::Key> selectSubKeys_{};
    std::vector<OriGine::PadButton> selectSubPadButtons_{};

    // ボタンを決定するためのキー ボタン
    std::vector<OriGine::Key> decideKeys_{};
    std::vector<OriGine::PadButton> decidePadButtons_{};

public:
    int32_t GetCurrentButtonNumber() const { return currentButtonNumber_; }
    void SetCurrentButtonNumber(int32_t _buttonNumber) { currentButtonNumber_ = _buttonNumber; }
    int32_t GetStartButtonNumber() const { return startButtonNumber_; }
    void SetStartButtonNumber(int32_t _buttonNumber) { startButtonNumber_ = _buttonNumber; }

    std::unordered_map<int32_t, OriGine::EntityHandle>& GetButtonNumbers() { return buttonNumbers_; }

    int32_t GetButtonNumber(OriGine::EntityHandle _entityId) const {
        for (const auto& pair : buttonNumbers_) {
            if (pair.second == _entityId) {
                return pair.first;
            }
        }
        return -1; // 見つからなかった場合のデフォルト値
    }
    OriGine::EntityHandle GetEntityId(int32_t _buttonNumber) const {
        auto it = buttonNumbers_.find(_buttonNumber);
        if (it != buttonNumbers_.end()) {
            return it->second;
        }
        return OriGine::EntityHandle(); // 見つからなかった場合のデフォルト値
    }

    const std::vector<OriGine::Key>& GetSelectAddKeys() const { return selectAddKeys_; }
    const std::vector<OriGine::PadButton>& GetSelectAddPadButtons() const { return selectAddPadButtons_; }
    const std::vector<OriGine::Key>& GetSelectSubKeys() const { return selectSubKeys_; }
    const std::vector<OriGine::PadButton>& GetSelectSubPadButtons() { return selectSubPadButtons_; }
    const std::vector<OriGine::Key>& GetDecideKeys() const { return decideKeys_; }
    const std::vector<OriGine::PadButton>& GetDecidePadButtons() const { return decidePadButtons_; }
};
