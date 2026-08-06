#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <graphics/Window.hpp>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <imgui_internal.h>

#include <stdexcept>
#include <utility>  // using std::exchange

using namespace rasterizer::graphics;

struct SDL_Context
{
    static SDL_Context& get()
    {
        static SDL_Context instance;
        return instance;
    }

private:
    SDL_Context()
    {
        if ( !SDL_Init( SDL_INIT_VIDEO ) )
        {
            SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: &s", SDL_GetError() );
            throw std::runtime_error( SDL_GetError() );
        }
    }
    ~SDL_Context()
    {
        SDL_Quit();
    }
};

Window::~Window()
{
    destroy();
}

Window::Window( std::string_view title, int width, int height, bool fullscreen )
{
    create( title, width, height, fullscreen );
}

Window::Window( Window&& window ) noexcept
: m_Window( std::exchange( window.m_Window, nullptr ) )
, m_Renderer( std::exchange( window.m_Renderer, nullptr ) )
, m_ImGuiContext( std::exchange( window.m_ImGuiContext, nullptr ) )
, m_Width( std::exchange( window.m_Width, -1 ) )
, m_Height( std::exchange( window.m_Height, -1 ) )
, m_Fullscreen( std::exchange( window.m_Fullscreen, false ) )
, m_VSync( std::exchange( window.m_VSync, true ) )
{
}

Window& Window::operator=( Window&& window ) noexcept
{
    if ( this == &window )
        return *this;
    m_Window       = std::exchange( window.m_Window, nullptr );
    m_Renderer     = std::exchange( window.m_Renderer, nullptr );
    m_ImGuiContext = std::exchange( window.m_ImGuiContext, nullptr );
    m_Width        = std::exchange( window.m_Width, -1 );
    m_Height       = std::exchange( window.m_Height, -1 );
    m_Fullscreen   = std::exchange( window.m_Fullscreen, false );
    m_VSync        = std::exchange( window.m_VSync, true );
    return *this;
}

Window::operator bool() const
{
    return isValid();
}

void Window::create( std::string_view title, int width, int height, bool fullscreen )
{
    static SDL_Context& context = SDL_Context::get();

    if ( m_Window )
    {
        destroy();
    }
    m_Fullscreen          = fullscreen;
    SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
    flags |= m_Fullscreen ? SDL_WINDOW_RESIZABLE : 0;
    if ( !SDL_CreateWindowAndRenderer( title.data(), width, height, flags, &m_Window, &m_Renderer ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to create window and renderer: %s", SDL_GetError() );
        throw std::runtime_error( SDL_GetError() );
    }
    if ( !SDL_AddEventWatch( &Window::eventWatch, this ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to add event watch: %s", SDL_GetError() );
        throw std::runtime_error( SDL_GetError() );
    }

    // Enable vsync
    SDL_SetRenderVSync( m_Renderer, m_VSync ? 1 : 0 );

    resize( width, height );

    m_ImGuiContext = ImGui::CreateContext();
    ImStrncpy( m_ImGuiContext->ContextName, title.data(), IM_ARRAYSIZE( m_ImGuiContext->ContextName ) );
    ImGui::SetCurrentContext( m_ImGuiContext );
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();
    float primaryDisplayScale = SDL_GetDisplayContentScale( SDL_GetDisplayForWindow( m_Window ) );
    ImGuiStyle& style               = ImGui::GetStyle();
    style.ScaleAllSizes( primaryDisplayScale );
    style.FontScaleDpi = primaryDisplayScale;
    io.ConfigDpiScaleViewports = true;

    ImGui_ImplSDL3_InitForSDLRenderer( m_Window, m_Renderer );
    ImGui_ImplSDLRenderer3_Init( m_Renderer );
    beginFrame();
}

void Window::destroy()
{
    SDL_RemoveEventWatch( &Window::eventWatch, this );

    if ( m_ImGuiContext )
    {
        ImGuiContext* previousContext = ImGui::GetCurrentContext();
        if ( previousContext == m_ImGuiContext )
        {
            previousContext = nullptr;
        }
        ImGui::SetCurrentContext( m_ImGuiContext );
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext( m_ImGuiContext );
        m_ImGuiContext = nullptr;

        ImGui::SetCurrentContext( previousContext );

    }


    SDL_DestroyRenderer( m_Renderer );
    SDL_DestroyWindow( m_Window );

    m_Window     = nullptr;
    m_Renderer   = nullptr;
    m_Width      = -1;
    m_Height     = -1;
    m_Fullscreen = false;
    m_VSync      = true;
}

void Window::close()
{
    if ( !m_Window )
        return;
    m_Close = true;
    SDL_HideWindow( m_Window );
}

bool Window::isValid() const
{
    return m_Window && !m_Close;
}

void Window::clear( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
{
    if ( !m_Renderer )
        return;
    SDL_SetRenderDrawColor( m_Renderer, r, g, b, a );
    SDL_RenderClear( m_Renderer );
}

void Window::present()
{
    if ( !m_ImGuiContext || !m_Renderer )
        return;
    ImGui::SetCurrentContext( m_ImGuiContext );
    ImGui::Render();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    ImGui_ImplSDLRenderer3_RenderDrawData( ImGui::GetDrawData(), m_Renderer );
    if ( !SDL_RenderPresent( m_Renderer ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to present: %s", SDL_GetError() );
    }
    beginFrame();
}

void Window::resize( int width, int height )
{
    if ( m_Width != width || m_Height != height )
    {
        width  = std::max( 1, width );
        height = std::max( 1, height );

        if ( !SDL_SetWindowSize( m_Window, width, height ) )
        {
            SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to resize the window: %s", SDL_GetError() );
            return;
        }

        m_Width  = width;
        m_Height = height;
    }
}

void Window::setFullscreen( bool fullscreen )
{
    SDL_SetWindowFullscreen( m_Window, fullscreen );
    m_Fullscreen = fullscreen;
}

void Window::toggleFullscreen()
{
    setFullscreen( !m_Fullscreen );
}

bool Window::isFullscreen() const noexcept
{
    return m_Fullscreen;
}

void Window::setVSync( bool vSync )
{
    if ( m_Renderer )
    {
        SDL_SetRenderVSync( m_Renderer, vSync );
        m_VSync = vSync;
    }
}

void Window::toggleVSync()
{
    setVSync( !m_VSync );
}

bool Window::isVSync() const noexcept
{
    return m_VSync;
}

bool Window::setCurrent()
{
    if ( m_ImGuiContext )
    {
        ImGui::SetCurrentContext( m_ImGuiContext );
        return true;
    }
    return false;
}

bool SDLCALL Window::eventWatch( void* userdata, SDL_Event* event )
{
    Window* self = static_cast<Window*>( userdata );

    struct ContextSwitcher
    {
        ContextSwitcher( ImGuiContext* newContext )
        {
            previousContext = ImGui::GetCurrentContext();
            ImGui::SetCurrentContext( newContext );
        }
        ~ContextSwitcher()
        {
            ImGui::SetCurrentContext( previousContext );
        }
        ImGuiContext* previousContext;
    } switcher( self->m_ImGuiContext);

    ImGui_ImplSDL3_ProcessEvent( event );
    switch ( event->type )
    {
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        if ( event->window.windowID == SDL_GetWindowID( self->m_Window ) )
        {
            self->close();
        }
        break;
    case SDL_EVENT_WINDOW_RESIZED:
        if ( event->window.windowID == SDL_GetWindowID( self->m_Window ) )
        {
            self->m_Width  = event->window.data1;
            self->m_Height = event->window.data2;
        }
        break;
    case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
    case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
    {
        SDL_DisplayID id = SDL_GetDisplayForWindow( self->m_Window );
        float         scale = SDL_GetDisplayContentScale( id );

        ImGuiStyle style;
        ImGui::StyleColorsDark( &style );
        style.ScaleAllSizes( scale );
        style.FontScaleDpi = scale;
        ImGui::GetStyle()  = style;
    }
    break;
    }

    return true;
}

void Window::beginFrame()
{
    ImGui::SetCurrentContext( m_ImGuiContext );
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}
