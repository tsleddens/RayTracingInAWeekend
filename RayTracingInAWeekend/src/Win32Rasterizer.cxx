#include "Win32Rasterizer.h"

#include <mutex>

using namespace tsleddens;

Win32Rasterizer::Win32Rasterizer( UINT width, UINT height, const wchar_t* title )
: m_width( width )
, m_height( height )
, m_aspectRatio( static_cast<float>( width ) / static_cast<float>( height ) )
, m_hwnd( CreateWindowHandle( title ) )
, m_colorAccumulator( width, height )
, m_bufferedGdiBitmap( width, height, m_hwnd )
{}

Win32Rasterizer::~Win32Rasterizer()
{
    m_isRunning = false;
    if ( m_renderThread.joinable() )
    {
        m_renderThread.join();
    }
}

int Win32Rasterizer::Run( int cmdShow )
{
    OnInit();
    ShowWindow( m_hwnd, cmdShow );

    m_renderThread = std::thread( [this]() {
        while ( m_isRunning )
        {
            if ( m_isResizing )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                continue;
            }

            OnUpdate();
            {
                std::scoped_lock lock( m_renderLock );
                OnBeforeRender();
            }

            InvalidateRect( m_hwnd, nullptr, FALSE );
            OnAfterRender();
        }
    } );

    const int exitCode = StartMessageLoop();
    if ( m_renderThread.joinable() )
    {
        m_renderThread.join();
    }
    OnDestroy();
    return exitCode;
}

void Win32Rasterizer::SetWindowTitle( const wchar_t* title ) const
{
    SetWindowText( m_hwnd, title );
}

void Win32Rasterizer::Resize( UINT width, UINT height )
{
    if ( width > 0 && height > 0 && ( width != m_width || height != m_height ) )
    {
        {
            std::scoped_lock lock( m_renderLock );
            m_aspectRatio = static_cast<float>( width ) / static_cast<float>( height );

            m_bufferedGdiBitmap.Resize( width, height );
            m_colorAccumulator.Resize( width, height );

            m_width  = width;
            m_height = height;
        }
        OnResize( width, height );
    }
}

RECT Win32Rasterizer::GetClientSurface() const
{
    RECT clientSurface = {};
    GetClientRect( m_hwnd, &clientSurface );
    return clientSurface;
}

void Win32Rasterizer::Render()

{
    std::scoped_lock lock( m_renderLock );
    m_bufferedGdiBitmap.Paint();
}

LRESULT Win32Rasterizer::StaticWindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    Win32Rasterizer* pApp = nullptr;

    if ( msg == WM_CREATE )
    {
        const auto pCreateStruct = reinterpret_cast<LPCREATESTRUCT>( lParam );
        pApp                     = static_cast<Win32Rasterizer*>( pCreateStruct->lpCreateParams );
        SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pApp ) );
    }
    else
    {
        pApp = reinterpret_cast<Win32Rasterizer*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
    }

    return pApp ? pApp->WindowProc( hWnd, msg, wParam, lParam ) : DefWindowProc( hWnd, msg, wParam, lParam );
}

LRESULT Win32Rasterizer::WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch ( msg )
    {
    case WM_CREATE:
    {
        const auto pCreateStruct = reinterpret_cast<LPCREATESTRUCT>( lParam );
        SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pCreateStruct->lpCreateParams ) );
    }
        return 0;
    case WM_ENTERSIZEMOVE:
        m_isResizing = true;
        return 0;
    case WM_EXITSIZEMOVE:
        m_isResizing = false;
        return 0;
    case WM_PAINT:
        Render();
        return 0;
    case WM_SIZE:
    {
        UINT width  = LOWORD( lParam );
        UINT height = HIWORD( lParam );
        Resize( width, height );
    }
        return 0;
    case WM_DESTROY:
        PostQuitMessage( 0 );
        return 0;
    default:
        return DefWindowProc( hWnd, msg, wParam, lParam );
    }
}

HWND Win32Rasterizer::CreateWindowHandle( const wchar_t* title )
{
    const auto windowClass = RegisterWindowClass( StaticWindowProc );
    RECT       windowRect  = { 0, 0, static_cast<long>( m_width ), static_cast<long>( m_height ) };
    AdjustWindowRect( &windowRect, WS_OVERLAPPEDWINDOW, FALSE );

    const auto window =
        CreateWindow( windowClass.lpszClassName, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                      windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
                      nullptr,  // We have no parent window.
                      nullptr,  // We aren't using menus.
                      GetInstanceHandle(), this );
    assert( window != nullptr );
    return window;
}

WNDCLASSEX Win32Rasterizer::RegisterWindowClass( WNDPROC messageHandlerCallback ) const
{
    WNDCLASSEX windowClass    = {};
    windowClass.cbSize        = sizeof( WNDCLASSEX );
    windowClass.style         = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc   = messageHandlerCallback;
    windowClass.hInstance     = GetInstanceHandle();
    windowClass.hCursor       = LoadCursor( NULL, IDC_ARROW );
    windowClass.lpszClassName = L"WindowClass";

    const auto result = RegisterClassEx( &windowClass );
    assert( result != 0 );
    return windowClass;
}

int Win32Rasterizer::StartMessageLoop()
{
    MSG msg = {};
    while ( msg.message != WM_QUIT )
    {
        if ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }
    m_isRunning = false;
    return static_cast<int>( msg.wParam );
}