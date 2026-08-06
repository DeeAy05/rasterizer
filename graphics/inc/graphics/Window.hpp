#pragma once

#include <SDL3/SDL_events.h>
#include <string_view>


struct SDL_Window;
struct SDL_Renderer;
struct ImGuiContext;

namespace rasterizer
{
inline namespace graphics
{
class Window
{
public:
    Window() = default;
    ~Window();

    Window( std::string_view title, int width, int height, bool fullscreen = false );
    Window( const Window& ) = delete;
    Window( Window&& window ) noexcept;

    Window& operator=( const Window& ) = delete;
    Window& operator=( Window&& ) noexcept;

    explicit operator bool() const;

    void create( std::string_view title, int width, int height, bool fullscreen = false );

    void destroy();

    void close();

    bool isValid() const;

    void clear( uint8_t r, uint8_t b, uint8_t g, uint8_t a = 255 );

    void present();

    void resize( int width, int height );

    void setFullscreen( bool fullscreen );

    void toggleFullscreen();

    bool isFullscreen() const noexcept;

    void setVSync( bool vSync );

    void toggleVSync();

    bool isVSync() const noexcept;

    bool setCurrent();

private:
    static bool SDLCALL eventWatch( void* userdata, SDL_Event* event );
    void                beginFrame();

    ImGuiContext* m_ImGuiContext = nullptr;
    SDL_Window*   m_Window;
    SDL_Renderer* m_Renderer;
    int           m_Width      = -1;
    int           m_Height     = -1;
    bool          m_Fullscreen = false;
    bool          m_VSync      = true;
    bool          m_Close      = false;

};  // class Window
}  // namespace graphics
}  // namespace rasterizer