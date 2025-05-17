#include "HelloSphere.h"

int CALLBACK wWinMain(HINSTANCE, HINSTANCE, PWSTR, int nCmdShow)
{
    const auto app = new HelloSphere(500, 500, L"Test");
    return app->Run(nCmdShow);
}

#if defined(_DEBUG)
int main()
{
    return wWinMain(GetModuleHandle(NULL), NULL, GetCommandLineW(), SW_SHOWNORMAL);
}
#endif
