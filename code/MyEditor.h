#pragma once
#include "FrameWork.h"
#ifdef _DEBUG

class EditorController;
class MyEditor
    : public FrameWork {
public:
    MyEditor();
    virtual ~MyEditor();

    virtual void Initialize() override;
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
