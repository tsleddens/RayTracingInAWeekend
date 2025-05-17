#include "Win32Rasterizer.h"

using namespace tsleddens;

Win32Rasterizer::Win32Rasterizer(UINT width, UINT height, const wchar_t* title) :
    m_width(width),
    m_height(height),
    m_aspectRatio(static_cast<float>(width) / static_cast<float>(height)),
    m_hwnd(CreateWindowHandle(title)),
    m_hdc(GetDC(GetWindowHandle())),
    m_hdcMem(CreateCompatibleDC(m_hdc)),
    m_ppBackBufferRows(),
    m_pBackBufferPixels(),
    m_hBackBuffer(CreateBackBuffer(width, height, false))
{
}

Win32Rasterizer::~Win32Rasterizer()
{
    DestroyBackBuffer();

    DeleteDC(m_hdcMem);
    DeleteDC(m_hdc);
}

int Win32Rasterizer::Run(int cmdShow)
{
    OnInit();
    ShowWindow(m_hwnd, cmdShow);
    const int exitCode = StartMessageLoop();
    OnDestroy();

    return exitCode;
}

void Win32Rasterizer::PlotPixel(UINT x, UINT y, ColorCode pixel) const
{
    m_ppBackBufferRows[y][x] = pixel;
}

void Win32Rasterizer::PlotPixel(UINT x, UINT y, const Color& color) const
{
    m_ppBackBufferRows[y][x] = ColorToColorCode(color);
}

void Win32Rasterizer::Resize(UINT width, UINT height)
{
    if (width > 0 && height > 0 && (width != m_width || height != m_height))
    {
        m_width = width;
        m_height = height;
        m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        m_hBackBuffer = CreateBackBuffer(width, height, true);
    }
}

RECT Win32Rasterizer::GetClientSurface() const
{
    RECT clientSurface = {};
    GetClientRect(m_hwnd, &clientSurface);
    return clientSurface;
}

void Win32Rasterizer::DestroyBackBuffer()
{
    _aligned_free(static_cast<void*>(m_ppBackBufferRows));
    DeleteObject(m_hBackBuffer);
}

void Win32Rasterizer::Render() const
{
    BITMAP bitmap;
    GetObject(m_hBackBuffer, sizeof(BITMAP), &bitmap);

    SelectObject(m_hdcMem, m_hBackBuffer);

    BitBlt(m_hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, m_hdcMem, 0, 0, SRCCOPY);
}

HBITMAP Win32Rasterizer::CreateBackBuffer(UINT width, UINT height, bool reAlloc)
{
    BITMAPINFO bitmapInfo;
    bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.bmiHeader.biWidth = static_cast<long>(width);
    bitmapInfo.bmiHeader.biHeight = static_cast<long>(height);
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = s_BitCount;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    HBITMAP hBitmap = CreateDIBSection(nullptr, &bitmapInfo, 0, reinterpret_cast<void**>(&m_pBackBufferPixels), nullptr, 0);
    assert(hBitmap != nullptr);


    UINT alignment = sizeof(UINT*);
    UINT size = height * alignment;
    m_ppBackBufferRows = static_cast<unsigned int**>(reAlloc
        ? _aligned_realloc(static_cast<void*>(m_ppBackBufferRows), size, alignment)
        : _aligned_malloc(size, alignment)
    );

    if (m_ppBackBufferRows != nullptr)
    {
        for(UINT y = 0; y < height; ++y)
        {
            const UINT rowBeginIndex = y * m_width;
            m_ppBackBufferRows[y] = &m_pBackBufferPixels[rowBeginIndex];
        }
    }

    return hBitmap;
}

LRESULT Win32Rasterizer::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    const auto pApp = reinterpret_cast<Win32Rasterizer*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (msg)
    {
    case WM_CREATE:
        {
            const auto pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        }
    return 0;
    case WM_PAINT:
        if (pApp != nullptr)
        {
            pApp->OnUpdate();
            pApp->OnBeforeRender();
            pApp->Render();
            pApp->OnAfterRender();
        }
        return 0;
    case WM_SIZE:
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            pApp->Resize(width, height);
            return 0;
        }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

HWND Win32Rasterizer::CreateWindowHandle(const wchar_t* title)
{
    const auto windowClass = RegisterWindowClass(WindowProc);
    RECT windowRect = { 0, 0, static_cast<long>(m_width), static_cast<long>(m_height) };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    const auto window = CreateWindow(
        windowClass.lpszClassName,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr, // We have no parent window.
        nullptr, // We aren't using menus.
        GetInstanceHandle(),
        this);
    assert(window != nullptr);
    return window;
}

WNDCLASSEX Win32Rasterizer::RegisterWindowClass(WNDPROC messageHandlerCallback) const
{
    WNDCLASSEX windowClass = {};
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = messageHandlerCallback;
    windowClass.hInstance = GetInstanceHandle();
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = L"WindowClass";

    const auto result = RegisterClassEx(&windowClass);
    assert(result != 0);
    return windowClass;
}

int Win32Rasterizer::StartMessageLoop()
{
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    const int exitCode = static_cast<int>(msg.wParam);
    return exitCode;
}
