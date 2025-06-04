#include "BufferedGdiBitmap.h"

using namespace tsleddens;

BufferedGdiBitmap::BufferedGdiBitmap(UINT width, UINT height, HWND hwnd) :
    m_backBuffer(width, height),
    m_pFrontBuffer(nullptr),
    m_width(width),
    m_height(height),
    m_hwnd(hwnd)
{
    Resize(width, height);
}

BufferedGdiBitmap::~BufferedGdiBitmap()
{
    DeleteGdiResources();
}

void BufferedGdiBitmap::Paint() const
{
    PAINTSTRUCT ps{};
    HDC hdc = BeginPaint(m_hwnd, &ps);

    CopyToFrontBuffer();
    {
        BitBlt(hdc, 0, 0, m_width, m_height, m_hdcMem, 0, 0, SRCCOPY);
    }

    EndPaint(m_hwnd, &ps);
}

void BufferedGdiBitmap::Resize(UINT width, UINT height)
{
    DeleteGdiResources();

    HDC hdc = GetDC(m_hwnd);

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = static_cast<long>(width);
    bmi.bmiHeader.biHeight = static_cast<long>(height);
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    m_hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &m_pFrontBuffer, nullptr, 0);
    m_hdcMem = CreateCompatibleDC(hdc);
    SelectObject(m_hdcMem, m_hBitmap);
    ReleaseDC(m_hwnd, hdc);

    BITMAP bmpInfo = {};
    GetObject(m_hBitmap, sizeof(bmi), &bmpInfo);

    m_backBuffer.Resize(width, height);

    m_width = width;
    m_height = height;
}

void BufferedGdiBitmap::DeleteGdiResources()
{
    if (m_hBitmap)
    {
        DeleteObject(m_hBitmap);
        m_hBitmap = nullptr;
    }
    if (m_hdcMem)
    {
        DeleteDC(m_hdcMem);
        m_hdcMem = nullptr;
    }
}
