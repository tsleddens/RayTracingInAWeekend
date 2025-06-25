#include "SDLRasterizer.h"

#include <SDL3/SDL.h>

using namespace tsleddens;

SDLRasterizer::SDLRasterizer( const UINT width, const UINT height, const wchar_t* title )
: m_pWindow( nullptr )
, m_pRenderer( nullptr )
, m_pTexture( nullptr )
, m_width( width )
, m_height( height )
, m_colorAccumulator( width, height )
, m_frameBuffer( width, height )
{
    auto isInitialized = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS );
    if ( !isInitialized )
    {
        SPDLOG_ERROR( "Failed to init SDL: {}", SDL_GetError() );
    }

    m_pWindow   = SDL_CreateWindow( WideToUtf8( title ).c_str(), width, height, 0 );
    m_pRenderer = SDL_CreateRenderer( m_pWindow, NULL );
    m_pTexture = SDL_CreateTexture( m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height );
}

void SDLRasterizer::SetWindowTitle( const wchar_t* title ) const
{
    if ( m_isRunning )
    {
        SDL_SetWindowTitle( m_pWindow, WideToUtf8( title ).c_str() );
    }
}

void SDLRasterizer::Resize( UINT width, UINT height ) {}

int SDLRasterizer::StartMessageLoop()
{
    while ( m_isRunning )
    {
        SDL_Event e;
        if ( SDL_PollEvent( &e ) )
        {
            if ( e.type == SDL_EVENT_QUIT )
            {
                m_isRunning = false;
                return 0;
            }
        }
        SDL_Delay( 1000 / 60 );

        OnUpdate();
    }

    return -1;
}

void SDLRasterizer::Render() const
{
    SDL_UpdateTexture( m_pTexture, nullptr, m_frameBuffer.Raw(), m_width * sizeof( ColorCode ) );
    SDL_RenderTexture( m_pRenderer, m_pTexture, NULL, NULL );
    SDL_RenderPresent( m_pRenderer );
}

int SDLRasterizer::Run( int cmdShow )
{
    OnInit();

    m_renderThread = std::thread( [this]() {
        while ( m_isRunning )
        {
            if ( m_isResizing )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                continue;
            }

            {
                std::scoped_lock lock( m_renderLock );
                OnBeforeRender();
                Render();
                OnAfterRender();
            }
        }
    } );

    const int exitCode = StartMessageLoop();
    if ( m_renderThread.joinable() )
    {
        m_renderThread.join();
    }
    OnDestroy();
    return exitCode;
}

SDLRasterizer::~SDLRasterizer()
{
    m_isRunning = false;
    if ( m_renderThread.joinable() )
    {
        m_renderThread.join();
    }

    SDL_DestroyTexture( m_pTexture );
    SDL_DestroyRenderer( m_pRenderer );
    SDL_DestroyWindow( m_pWindow );
    SDL_Quit();
}