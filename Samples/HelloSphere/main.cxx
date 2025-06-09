#include "BouncingSpheres.h"
#include "HelloSphere.h"
#include "spdlog/sinks/stdout_color_sinks.h"

int CALLBACK wWinMain(HINSTANCE, HINSTANCE, PWSTR, int nCmdShow)
{
    auto console = spdlog::stdout_color_mt("console");
    // auto app = HelloSphere(1024 / 2, 768 / 2, L"Test");
    auto app = BouncingSpheres(1024 / 2, 768 / 2, L"Test");
    return app.Run(nCmdShow);
}

#if defined(_DEBUG)
int main()
{
    return wWinMain(GetModuleHandle(NULL), NULL, GetCommandLineW(), SW_SHOWNORMAL);
}
#endif
