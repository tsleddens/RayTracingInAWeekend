#pragma once
#include "Aligned2DArray.h"
#include "IRasterizer.h"
#include <SDL3/SDL.h>

namespace tsleddens
{
class SDLRasterizer : public IRasterizer
{
    SDL_Window*   m_pWindow;
    SDL_Renderer* m_pRenderer;
    SDL_Texture*  m_pTexture;

    UINT m_width;
    UINT m_height;

    Aligned2DArray<Color>     m_colorAccumulator;
    Aligned2DArray<ColorCode> m_frameBuffer;

    std::mutex  m_renderLock;
    std::thread m_renderThread;

    std::atomic<bool> m_isRunning { true };
    std::atomic<bool> m_isResizing { false };

public:
    int Run( int cmdShow ) override;

    void PlotPixel( const UINT x, const UINT y, const Color& color, const float reciprocalFrameCount ) override
    {
        m_colorAccumulator[y][x] += ( color - m_colorAccumulator[y][x] ) * reciprocalFrameCount;
        m_frameBuffer[y][x] = ColorToColorCodeSdl( m_colorAccumulator[y][x] );
    }

    ~SDLRasterizer() override;

protected:
    SDLRasterizer( UINT width, UINT height, const wchar_t* title );

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
        return m_width * m_height;
    }

    [[nodiscard]] float GetAspectRatio() const override
    {
        return static_cast<float>( m_width ) / static_cast<float>( m_height );
    }

private:
    int  StartMessageLoop();
    void Render() const;
};
}  // namespace tsleddens