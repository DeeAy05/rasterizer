#include <SDL3/SDL.h>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGTH = 600;
constexpr const char* TITLE = "01 - ClearScreen";

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
    if (!SDL_CreateWindowAndRenderer(TITLE, SCREEN_WIDTH, SCREEN_HEIGTH, flags, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window and renderer: %s", SDL_GetError());
        return -2;
    }
    bool vSync = true;
    SDL_SetRenderVSync(renderer, vSync ? 1 : 0);
    bool running = true;
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch (e.key.key) 
                {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_V:
                    vSync = !vSync;
                    SDL_SetRenderVSync(renderer, vSync ? 1 : 0);
                    break;
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 154, 206, 235, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}