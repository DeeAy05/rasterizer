#include <graphics/Window.hpp>

constexpr int         SCREEN_WIDTH  = 800;
constexpr int         SCREEN_HEIGHT = 600;
constexpr const char* TITLE         = "01 - ClearScreen";

using namespace rasterizer::graphics;

int main()
{
    Window window( TITLE, SCREEN_WIDTH, SCREEN_HEIGHT );
    while ( window )
    {
        SDL_Event e;
        while ( SDL_PollEvent( &e ) )
        {
            switch ( e.type )
            {
            case SDL_EVENT_QUIT:
                window.close();
                break;
            case SDL_EVENT_KEY_DOWN:
                switch ( e.key.key )
                {
                case SDLK_ESCAPE:
                    window.close();
                    break;
                case SDLK_V:
                    window.toggleVSync();
                    break;
                case SDLK_RETURN:
                    if ( ( e.key.mod & SDL_KMOD_ALT ) != 0 )
                    {
                        case SDLK_F11:
                        window.toggleFullscreen();
                    }
                        break;
                }
                break;
            }
        }
        window.clear( 154, 206, 235 );
        window.present();
    }
    return 0;
}