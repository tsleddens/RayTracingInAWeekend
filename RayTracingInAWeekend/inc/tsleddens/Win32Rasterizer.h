#pragma once

#include "Defines.h"

namespace tsleddens
{
    constexpr int s_BitCount = 32; //don't change

    class Win32Rasterizer
    {
        UINT m_width;
        UINT m_height;
        float m_aspectRatio;

        HWND m_hwnd;

        HDC m_hdc;
        HDC m_hdcMem;

    public:
        unsigned int** m_ppBackBufferRows;
        unsigned int* m_pBackBufferPixels;

        HBITMAP m_hBackBuffer;


        virtual ~Win32Rasterizer();

        int Run(int cmdShow);

    protected:
        Win32Rasterizer(UINT width, UINT height, const wchar_t* title);
        void PlotPixel(UINT x, UINT y, ColorCode pixel) const;
        void PlotPixel(UINT x, UINT y, const Color& color) const;

        virtual void OnInit() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnBeforeRender() = 0;
        virtual void OnAfterRender() = 0;
        virtual void OnDestroy() = 0;

        void SetWindowTitle(const wchar_t* title) const { SetWindowText(m_hwnd, title); }
        void Resize(UINT width, UINT height);

        [[nodiscard]] UINT GetWidth() const { return m_width; }
        [[nodiscard]] UINT GetHeight() const { return m_height; }
        [[nodiscard]] UINT GetPixelCount() const { return m_height * m_width; }
        [[nodiscard]] float GetAspectRatio() const { return m_aspectRatio; }
        [[nodiscard]] ColorCode GetColorCode(int x, int y) const { return m_ppBackBufferRows[y][x]; }
        [[nodiscard]] Color GetColor(int x, int y) const { return ColorCodeToColor(m_ppBackBufferRows[y][x]); }

    private:
        void Render() const;

        HBITMAP CreateBackBuffer(UINT width, UINT height, bool reAlloc);
        void DestroyBackBuffer();

        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        HWND CreateWindowHandle(const wchar_t* title);
        WNDCLASSEX RegisterWindowClass(WNDPROC messageHandlerCallback) const;
        int StartMessageLoop();

        [[nodiscard]] RECT GetClientSurface() const;
        [[nodiscard]] HDC GetDeviceContextHandle() const { return m_hdc; }
        [[nodiscard]] HWND GetWindowHandle() const { return m_hwnd; }
        [[nodiscard]] HINSTANCE GetInstanceHandle() const { return GetModuleHandle(nullptr); }

    };
}
