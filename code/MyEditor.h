#pragma once
#include "FrameWork.h"

class EditorController;
class MyEditor
    : public FrameWork {
public:
    MyEditor();
    virtual ~MyEditor();

    virtual void Initialize(const std::string& _commandLine) override;
    virtual void Finalize() override;
    virtual void Run() override;

protected:
    bool isEndRequest_                  = false;
    EditorController* editorController_ = nullptr;

public:
    bool isEndRequest() const { return isEndRequest_; }
    void setEndRequest(bool endRequest) { isEndRequest_ = endRequest; }
};
