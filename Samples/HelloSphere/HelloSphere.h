#pragma once

#include "tsleddens/AppBase.h"
#include "tsleddens/World.h"
#include "tsleddens/Textures/ITexture.h"

class HelloSphere final : public tsleddens::AppBase
{
    std::shared_ptr<tsleddens::ITexture> m_checkerTexture;
    std::unique_ptr<tsleddens::IMaterial> m_materialGround;
    std::unique_ptr<tsleddens::IMaterial> m_material1;
    std::unique_ptr<tsleddens::IMaterial> m_material2;
    std::unique_ptr<tsleddens::IMaterial> m_material3;
    std::unique_ptr<tsleddens::IMaterial> m_materials[529];

public:
    HelloSphere(int width, int height, const wchar_t* title);

    ~HelloSphere() override = default;

protected:
    void InitWorld(tsleddens::World& world) override;
};
