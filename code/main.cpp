#include "engine/code/directX12/DxDebug.h"

#include "application/code/MyEditor.h"
#include "application/code/MyGame.h"

#include "lib/logger/Logger.h"

#include <memory>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    DxDebug::getInstance()->InitializeDebugger();

    Logger::Initialize();

#ifdef _DEBUG
    {
        std::unique_ptr<MyEditor> editorApp = std::make_unique<MyEditor>();

        editorApp->Initialize();
        DxDebug::getInstance()->CreateInfoQueue();

        editorApp->Run();

        editorApp->Finalize();
    }
#else
    {
        std::unique_ptr<MyGame> gameApp = std::make_unique<MyGame>();

        gameApp->Initialize();
        DxDebug::getInstance()->CreateInfoQueue();

        gameApp->Run();

        gameApp->Finalize();
    }
#endif // _DEBUG

    Logger::Finalize();

    return 0;
}
