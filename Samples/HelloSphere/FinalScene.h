#pragma once
#include "tsleddens/AppBase.h"
#include "tsleddens/ConstantMedium.h"
#include "tsleddens/Materials/Dielectric.h"
#include "tsleddens/Materials/DiffuseLight.h"
#include "tsleddens/Materials/Lambertian.h"
#include "tsleddens/Materials/Metal.h"
#include "tsleddens/Quad.h"
#include "tsleddens/RotateY.h"
#include "tsleddens/Sphere.h"
#include "tsleddens/Textures/ImageTexture.h"
#include "tsleddens/Textures/NoiseTexture.h"
#include "tsleddens/Translate.h"

using namespace tsleddens;

class FinalScene final : public AppBase
{
    std::shared_ptr<ITexture>      m_earthTexture;
    std::shared_ptr<ITexture>      m_noiseTexture;
    std::unique_ptr<IMaterial>     m_ground;
    std::unique_ptr<IMaterial>     m_light;
    std::unique_ptr<IMaterial>     m_sphereMaterial;
    std::unique_ptr<IMaterial>     m_glassMaterial;
    std::unique_ptr<IMaterial>     m_metalMaterial;
    std::unique_ptr<IMaterial>     m_earthMaterial;
    std::unique_ptr<IMaterial>     m_noiseMaterial;
    std::shared_ptr<IMaterial>     m_emptyMaterial;
    std::unique_ptr<IRayTraceable> m_lights;

public:
    FinalScene( int width, int height, const wchar_t* title )
    : AppBase( width, height, title )
    , m_ground( std::make_unique<Lambertian>( 0.48f, 0.83f, 0.53f ) )
    , m_light( std::make_unique<DiffuseLight>( 7.f ) )
    , m_sphereMaterial( std::make_unique<Lambertian>( 0.7f, 0.3f, 0.1f ) )
    , m_glassMaterial( std::make_unique<Dielectric>( 1.5f ) )
    , m_metalMaterial( std::make_unique<Metal>( Color( .8f, .8f, .9f ), 1.f ) )
    , m_earthTexture( std::make_shared<ImageTexture>( "images/earthmap.jpg" ) )
    , m_earthMaterial( std::make_unique<Lambertian>( m_earthTexture ) )
    , m_noiseTexture( std::make_shared<NoiseTexture>( 0.2f ) )
    , m_noiseMaterial( std::make_unique<Lambertian>( m_noiseTexture ) )
    , m_emptyMaterial( std::shared_ptr<IMaterial>() )
    , m_lights( std::make_unique<Quad>( Point3( 123.f, 554.f, 147.f ), Vector3( 300.f, 0.f, 0.f ),
                                        Vector3( 0.f, 0.f, 265.f ), m_emptyMaterial.get() ) )
    {}

    ~FinalScene() override = default;

protected:
    void InitWorld( World& world ) override
    {
        const auto camera = GetCamera();
        camera->SetPosition( 478.f, 278.f, -600.f );
        camera->SetLookAt( 278.f, 278.f, 0.f );
        camera->SetVerticalFieldOfView( 40.f );
        camera->SetMaxBounces( 40 );

        constexpr int boxesPerSide = 20;

        for ( int i = 0; i < boxesPerSide; ++i )
            for ( int j = 0; j < boxesPerSide; ++j )
            {
                constexpr float w  = 100.f;
                const float     x0 = -1000.f + static_cast<float>( i ) * w;
                const float     z0 = -1000.f + static_cast<float>( j ) * w;
                constexpr float y0 = 0.f;
                const float     x1 = x0 + w;
                const float     y1 = RandomFloat( 1.f, 101.f );
                const float     z1 = z0 + w;
                world.AddObject<Box>( Point3( x0, y0, z0 ), Point3( x1, y1, z1 ), m_ground.get() );
            }

        world.AddObject<Quad>( Point3( 123.f, 554.f, 147.f ), Vector3( 300.f, 0.f, 0.f ), Vector3( 0.f, 0.f, 265.f ),
                               m_light.get() );
        world.AddObject<Sphere>( Point3( 415.f, 400.f, 200.f ), 50.f, m_sphereMaterial.get() );

        world.AddObject<Sphere>( Point3( 250.f, 150.f, 45.f ), 50.f, m_glassMaterial.get() );
        world.AddObject<Sphere>( Point3( 0.f, 150.f, 145.f ), 50.f, m_metalMaterial.get() );

        std::shared_ptr<Sphere> boundary =
            std::make_shared<Sphere>( Point3( 360.f, 150.f, 145.f ), 70.f, m_glassMaterial.get() );
        world.AddObject( boundary );
        world.AddObject( std::make_shared<ConstantMedium>( boundary, 0.2f, Color( 0.2f, 0.4f, 0.9f ) ) );
        boundary = std::make_shared<Sphere>( Point3( 0.f ), 5000.f, m_glassMaterial.get() );
        world.AddObject( std::make_shared<ConstantMedium>( boundary, 0.00001f, Color( 1.f ) ) );

        world.AddObject<Sphere>( Point3( 400.f, 200.f, 400.f ), 100.f, m_earthMaterial.get() );
        world.AddObject<Sphere>( Point3( 220.f, 280.f, 300.f ), 80.f, m_noiseMaterial.get() );
    }

    [[nodiscard]] IRayTraceable* GetLights() override
    {
        return m_lights.get();
    }
};
