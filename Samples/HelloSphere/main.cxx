#include "BouncingSpheres.h"
#include "CornellBox.h"
#include "CornellSmoke.h"
#include "Earth.h"
#include "FinalScene.h"
#include "HelloSphere.h"
#include "PerlinSpheres.h"
#include "Quads.h"
#include "SimpleLight.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "tsleddens/RtwImage.h"

int CALLBACK wWinMain( HINSTANCE, HINSTANCE, PWSTR, int nCmdShow )
{
    auto console = spdlog::stdout_color_mt( "console" );
    // auto app = HelloSphere(1024 / 2, 768 / 2, L"Test");
    // auto app = BouncingSpheres(1024 / 2, 768 / 2, L"Test");
    // auto app = Earth(1024 / 2, 768 / 2, L"Test");
    // auto app = PerlinSpheres(1024 / 2, 768 / 2, L"Test");
    // auto app = Quads(1024 / 2, 768 / 2, L"Test");
    // auto app = SimpleLight(1024 / 2, 768 / 2, L"Test");
    auto app = CornellBox( 600, 600, L"Test" );
    // auto app = CornellSmoke(600, 600, L"Test");
    // auto app = FinalScene(600, 600, L"Test");
    return app.Run( nCmdShow );
}

#if defined( _DEBUG )
int main()
{
    spdlog::set_level( spdlog::level::err );
    return wWinMain( GetModuleHandle( NULL ), NULL, GetCommandLineW(), SW_SHOWNORMAL );
}
#endif
