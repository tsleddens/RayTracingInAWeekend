#pragma once

#include "tsleddens/BvhNode.h"
#include "tsleddens/Camera.h"
#include "tsleddens/Win32Rasterizer.h"
#include "tsleddens/World.h"
#include "tsleddens/Textures/ITexture.h"

class BouncingSpheres final : public tsleddens::Win32Rasterizer
{
    tsleddens::Camera m_camera;
    tsleddens::World m_world;
    tsleddens::BvhNode m_bvh;
    std::shared_ptr<tsleddens::ITexture> m_checkerTexture;
    std::unique_ptr<tsleddens::IMaterial> m_materialGround;

    ULONGLONG m_frameCount = 0;

public:
    BouncingSpheres(int width, int height, const wchar_t* title);

    ~BouncingSpheres() override = default;

protected:
    void OnInit() override {}
    void OnResize(UINT newWidth, UINT newHeight) override;
    void OnUpdate() override {}
    void OnBeforeRender() override;
    void OnAfterRender() override;
    void OnDestroy() override {}
private:
    void UpdateFps();
};
