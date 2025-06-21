#pragma once

#include "tsleddens/AppBase.h"
#include "tsleddens/Materials/DiffuseLight.h"
#include "tsleddens/Materials/Lambertian.h"
#include "tsleddens/Quad.h"
#include "tsleddens/Sphere.h"
#include "tsleddens/Textures/ITexture.h"
#include "tsleddens/Textures/NoiseTexture.h"
#include "tsleddens/World.h"

using namespace tsleddens;

class SimpleLight final : public AppBase
{
    std::shared_ptr<ITexture>  m_noiseTexture;
    std::unique_ptr<IMaterial> m_noiseMaterial;
    std::unique_ptr<IMaterial> m_lightMaterial;

public:
    SimpleLight( int width, int height, const wchar_t* title )
    : AppBase( width, height, title )
    , m_noiseTexture( std::make_shared<NoiseTexture>( 4.f ) )
    , m_noiseMaterial( std::make_unique<Lambertian>( m_noiseTexture ) )
    , m_lightMaterial( std::make_unique<DiffuseLight>( 4.f ) )
    {}

    ~SimpleLight() override = default;

protected:
    void InitWorld( World& world ) override
    {
        auto camera = GetCamera();
        camera->SetLookAt( Point3( 0.f, 2.f, 0.f ) );
        camera->SetPosition( Point3( 26.f, 3.f, 6.f ) );

        world.AddObject<Sphere>( Point3( 0.f, -1000.f, 0.f ), 1000.f, m_noiseMaterial.get() );
        world.AddObject<Sphere>( Point3( 0.f, 2.f, 0.f ), 2.f, m_noiseMaterial.get() );

        world.AddObject<Sphere>( Point3( 0.f, 7.f, 0.f ), 2.f, m_lightMaterial.get() );
        world.AddObject<Quad>( Point3( 3.f, 1.f, -2.f ), Vector3( 2.f, 0.f, 0.f ), Vector3( 0.f, 2.f, 0.f ),
                               m_lightMaterial.get() );
    }
};
