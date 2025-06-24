#pragma once

namespace tsleddens
{
struct IRasterizer
{
    virtual ~IRasterizer() = default;

    virtual int Run( int cmdShow ) = 0;

    virtual void PlotPixel( const UINT x, const UINT y, const Color& color, const float reciprocalFrameCount ) = 0;

protected:
    virtual void OnInit()                                     = 0;
    virtual void OnResize( UINT newWidth, UINT newHeight )    = 0;
    virtual void OnUpdate()                                   = 0;
    virtual void OnBeforeRender()                             = 0;
    virtual void OnAfterRender()                              = 0;
    virtual void OnDestroy()                                  = 0;
    virtual void SetWindowTitle( const wchar_t* title ) const = 0;
    virtual void Resize( UINT width, UINT height )            = 0;

    [[nodiscard]] virtual UINT  GetWidth() const       = 0;
    [[nodiscard]] virtual UINT  GetHeight() const      = 0;
    [[nodiscard]] virtual UINT  GetPixelCount() const  = 0;
    [[nodiscard]] virtual float GetAspectRatio() const = 0;
};
}  // namespace tsleddens
