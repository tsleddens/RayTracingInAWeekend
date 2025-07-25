#pragma once

#include "BufferedGdiBitmap.h"
#include "Defines.h"
#include "IRasterizer.h"

#include <mutex>

namespace tsleddens
{
constexpr int s_BitCount = 32;  // don't change

class Win32Rasterizer : public IRasterizer
{
    UINT m_width;
    UINT m_height;

    float m_aspectRatio;

    HWND m_hwnd;

    Aligned2DArray<Color> m_colorAccumulator;
    BufferedGdiBitmap     m_bufferedGdiBitmap;

    std::mutex  m_renderLock;
    std::thread m_renderThread;

    std::atomic<bool> m_isRunning { true };
    std::atomic<bool> m_isResizing { false };

public:
    ~Win32Rasterizer() override;

    int Run( int cmdShow ) override;

    void PlotPixel( const UINT x, const UINT y, const Color& color, const float reciprocalFrameCount ) override
    {
        m_colorAccumulator[y][x] += ( color - m_colorAccumulator[y][x] ) * reciprocalFrameCount;
        m_bufferedGdiBitmap.SetPixel( x, y, m_colorAccumulator[y][x] );
    }

protected:
    Win32Rasterizer( UINT width, UINT height, const wchar_t* title );

    void OnInit() override                                  = 0;
    void OnResize( UINT newWidth, UINT newHeight ) override = 0;
    void OnUpdate() override                                = 0;
    void OnBeforeRender() override                          = 0;
    void OnAfterRender() override                           = 0;
    void OnDestroy() override                               = 0;
    void SetWindowTitle( const wchar_t* title ) const override;
    void Resize( UINT width, UINT height ) override;

    [[nodiscard]] UINT GetWidth() const override
    {
        return m_width;
    }

    [[nodiscard]] UINT GetHeight() const override
    {
        return m_height;
    }

    [[nodiscard]] UINT GetPixelCount() const override
    {
        return m_height * m_width;
    }

    [[nodiscard]] float GetAspectRatio() const override
    {
        return m_aspectRatio;
    }

private:
    void Render();

    static LRESULT CALLBACK StaticWindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
    LRESULT CALLBACK        WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
    HWND                    CreateWindowHandle( const wchar_t* title );
    WNDCLASSEX              RegisterWindowClass( WNDPROC messageHandlerCallback ) const;
    int                     StartMessageLoop();

    [[nodiscard]] RECT GetClientSurface() const;

    [[nodiscard]] HWND GetWindowHandle() const
    {
        return m_hwnd;
    }

    [[nodiscard]] HINSTANCE GetInstanceHandle() const
    {
        return GetModuleHandle( nullptr );
    }
};
}  // namespace tsleddens