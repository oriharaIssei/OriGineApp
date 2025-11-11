#pragma once
#include "FrameWork.h"
#ifdef _DEBUG

class EditorController;
/// <summary>
/// 編集用のアプリケーションフレームワーク
/// </summary>
class MyEditor
    : public FrameWork {
public:
    MyEditor();
    virtual ~MyEditor();

    virtual void Initialize(const std::vector<std::string>& _commandLines) override;
    virtual void Finalize() override;
    virtual void Run() override;

protected:
    bool isEndRequest_                  = false;
    EditorController* editorController_ = nullptr;

public:
    bool isEndRequest() const { return isEndRequest_; }
    void setEndRequest(bool endRequest) { isEndRequest_ = endRequest; }
};

#endif // _DEBUG
