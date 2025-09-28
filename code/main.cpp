#include "engine/code/directX12/DxDebug.h"

#include "application/code/MyEditor.h"
#include "application/code/MyGame.h"

#include "logger/Logger.h"

#include <memory>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR _lpCmdLine, int) {
    DxDebug::getInstance()->InitializeDebugger();

    std::string cmdLine(_lpCmdLine);

    Logger::Initialize();

    {
        std::unique_ptr<MyEditor> editorApp = std::make_unique<MyEditor>();

        editorApp->Initialize(cmdLine);
        DxDebug::getInstance()->CreateInfoQueue();

        editorApp->Run();

        editorApp->Finalize();
    }

    Logger::Finalize();

    return 0;
}
