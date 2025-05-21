#pragma once

#include "tsleddens/Camera.h"
#include "tsleddens/Material.h"
#include "tsleddens/Win32Rasterizer.h"
#include "tsleddens/World.h"

class HelloSphere final : public tsleddens::Win32Rasterizer
{
    tsleddens::Camera m_camera;
    tsleddens::World m_world;
    tsleddens::Material m_material;

public:
    HelloSphere(int width, int height, const wchar_t* title);

    ~HelloSphere() override = default;

protected:
    void OnInit() override {}
    void OnResize(UINT newWidth, UINT newHeight) override;
    void OnUpdate() override;
    void OnBeforeRender() override;
    void OnAfterRender() override {}
    void OnDestroy() override {}
private:
    void UpdateFps() const;
};
