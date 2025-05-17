#pragma once

#include "tsleddens/Win32Rasterizer.h"

class HelloSphere final : public tsleddens::Win32Rasterizer
{
public:
    HelloSphere(int width, int height, const wchar_t* title)
        : Win32Rasterizer(width, height, title)
    {
    }

    ~HelloSphere() override = default;

protected:
    void OnInit() override {}
    void OnUpdate() override;
    void OnBeforeRender() override;
    void OnAfterRender() override {}
    void OnDestroy() override {}
private:
    void UpdateFps() const;
};