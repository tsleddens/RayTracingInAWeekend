#pragma once
#include "Aligned2DArray.h"

namespace tsleddens
{
class BufferedGdiBitmap
{
    HBITMAP m_hBitmap {};

    HDC m_hdcMem {};

    Aligned2DArray<ColorCode> m_backBuffer;
    void*                     m_pFrontBuffer;

    UINT m_width;
    UINT m_height;

    HWND m_hwnd;

public:
    BufferedGdiBitmap( UINT width, UINT height, HWND hwnd );
    ~BufferedGdiBitmap();

    void Paint() const;
    void Resize( UINT width, UINT height );

    void SetPixel( UINT x, UINT y, const Color& color )
    {
        m_backBuffer[y][x] = ColorToColorCode( color );
    }
    void CopyToFrontBuffer() const
    {
        m_backBuffer.CopyInto( m_pFrontBuffer );
    }

    [[nodiscard]] HBITMAP GetBitmap() const
    {
        return m_hBitmap;
    }
    [[nodiscard]] UINT GetWidth() const
    {
        return m_width;
    }
    [[nodiscard]] UINT GetHeight() const
    {
        return m_height;
    }

private:
    void DeleteGdiResources();
};
}  // namespace tsleddens
