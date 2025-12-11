
/// stl
#include <memory>
#include <string>
#include <vector>

/// engine
// directX12
#include "engine/code/directX12/DxDebug.h"

/// FrameWorks
#include "application/code/MyEditor.h"
#include "application/code/MyGame.h"

/// log
#include "logger/Logger.h"

std::vector<std::string> ParseCommandLine();

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    OriGine::DxDebug::GetInstance()->InitializeDebugger();

    std::vector<std::string> cmdLines = ParseCommandLine();

    OriGine::Logger::Initialize();

#ifdef DEBUG
    {
        std::unique_ptr<MyEditor> editorApp = std::make_unique<MyEditor>();

        editorApp->Initialize(cmdLines);
        OriGine::DxDebug::GetInstance()->CreateInfoQueue();

        editorApp->Run();

        editorApp->Finalize();
    }
#else
    {
        std::unique_ptr<MyGame> gameApp = std::make_unique<MyGame>();

        gameApp->Initialize(cmdLines);
        OriGine::DxDebug::GetInstance()->CreateInfoQueue();

        gameApp->Run();

        gameApp->Finalize();
    }
#endif // DEBUG

    OriGine::Logger::Finalize();

    return 0;
}

std::vector<std::string> ParseCommandLine() {
    int argc     = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    std::vector<std::string> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        // WideChar → UTF-8変換
        int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, nullptr, 0, nullptr, nullptr);
        std::string arg(sizeNeeded - 1, 0);
        WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, arg.data(), sizeNeeded, nullptr, nullptr);
        args.push_back(std::move(arg));
    }
    LocalFree(argv);

    return args;
}
