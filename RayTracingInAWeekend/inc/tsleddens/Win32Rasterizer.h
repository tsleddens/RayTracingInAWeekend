#pragma once

#include "BufferedGdiBitmap.h"
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

        Aligned2DArray<Color> m_colorAccumulator;
        BufferedGdiBitmap m_bufferedGdiBitmap;

        std::mutex m_renderLock;
        std::thread m_renderThread;

        std::atomic<bool> m_isRunning { true };
        std::atomic<bool> m_isResizing { false };
    public:


        virtual ~Win32Rasterizer();

        int Run(int cmdShow);

        void PlotPixel(UINT x, UINT y, const Color& color, float reciprocalFrameCount)
        {
            m_colorAccumulator[y][x] += (color - m_colorAccumulator[y][x]) * reciprocalFrameCount;
            m_bufferedGdiBitmap.SetPixel(x, y, m_colorAccumulator[y][x]);
        }

    protected:
        Win32Rasterizer(UINT width, UINT height, const wchar_t* title);

        virtual void OnInit() = 0;
        virtual void OnResize(UINT newWidth, UINT newHeight) = 0;
        virtual void OnUpdate() = 0;
        virtual void OnBeforeRender() = 0;
        virtual void OnAfterRender() = 0;
        virtual void OnDestroy() = 0;

        void SetWindowTitle(const wchar_t* title) const;
        void Resize(UINT width, UINT height);

        [[nodiscard]] UINT GetWidth() const { return m_width; }
        [[nodiscard]] UINT GetHeight() const { return m_height; }
        [[nodiscard]] UINT GetPixelCount() const { return m_height * m_width; }
        [[nodiscard]] float GetAspectRatio() const { return m_aspectRatio; }
        [[nodiscard]] Color GetColor(int x, int y) const { return m_colorAccumulator[y][x]; }

    private:
        void Render();

        static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        HWND CreateWindowHandle(const wchar_t* title);
        WNDCLASSEX RegisterWindowClass(WNDPROC messageHandlerCallback) const;
        int StartMessageLoop();

        [[nodiscard]] RECT GetClientSurface() const;
        [[nodiscard]] HWND GetWindowHandle() const { return m_hwnd; }
        [[nodiscard]] HINSTANCE GetInstanceHandle() const { return GetModuleHandle(nullptr); }

    };
}
