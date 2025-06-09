#pragma once

#include "tsleddens/AppBase.h"
#include "tsleddens/World.h"
#include "tsleddens/Textures/ITexture.h"

class BouncingSpheres final : public tsleddens::AppBase
{
    std::shared_ptr<tsleddens::ITexture> m_checkerTexture;
    std::unique_ptr<tsleddens::IMaterial> m_materialGround;

public:
    BouncingSpheres(int width, int height, const wchar_t* title);

    ~BouncingSpheres() override = default;

protected:
    void InitWorld(tsleddens::World& world) override;
};
