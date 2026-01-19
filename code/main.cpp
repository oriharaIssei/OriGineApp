
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
#include "FrameWork.h"

/// externals
#include "logger/Logger.h"

/// <summary>
/// コマンドラインを字句ごとに分解する
/// </summary>
/// <returns></returns>
std::vector<std::string> ParseCommandLine();

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    OriGine::DxDebug::GetInstance()->InitializeDebugger();

    std::vector<std::string> cmdLines = ParseCommandLine();

    OriGine::Logger::Initialize();

    std::unique_ptr<FrameWork> application = nullptr;

    // debug時は editor
    // develop ,release では Game を起動する
#ifdef DEBUG
    {
        application = std::make_unique<MyEditor>();
    }
#else
    {
        application = std::make_unique<MyGame>();
    }
#endif // DEBUG

    application->Initialize(cmdLines);
    OriGine::DxDebug::GetInstance()->CreateInfoQueue();

    application->Run();

    application->Finalize();
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
