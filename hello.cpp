#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    int texture_width ;
    int texture_height;
    float texture_x;
    float texture_y;
}AppState;


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    AppState *state = (AppState *)SDL_malloc(sizeof(AppState));
    state->window = NULL;
    state->renderer = NULL;
    state->texture = NULL;
    SDL_Surface *surface = NULL;
    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("Hello World", 800, 600, 0, &state->window, &state->renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_Log("Window: %p",(void *)state->window);
    SDL_Log("Renderer: %p",(void *)state->renderer);

    surface = SDL_LoadPNG("/img/Me.png");
    if(!surface){
        SDL_Log("Couldn't load png%s",SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_Log("PNG loaded: %d x %d", surface->w, surface->h);
    state->texture_width = surface->w;
    state->texture_height = surface->h;
    state->texture_x = (800-state->texture_width)/2.0f;
    state->texture_y = (600-state->texture_height)/2.0f;
    state->texture = SDL_CreateTextureFromSurface(state->renderer,surface); 
    SDL_DestroySurface(surface);
    *appstate = state;
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *state = (AppState *)appstate;
    if (event->type == SDL_EVENT_QUIT) {
        SDL_Log("Quit event received, exiting.");
        return SDL_APP_SUCCESS; 
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *state = (AppState *)appstate;
    const char *message = "Hello World!";
    int w = 0, h = 0;
    SDL_GetCurrentRenderOutputSize(state->renderer, &w, &h);

    const bool *key = SDL_GetKeyboardState(NULL);
    if(key[SDL_SCANCODE_W]){
        state->texture_y -= 2.0f;
    }if(key[SDL_SCANCODE_S]){
        state->texture_y += 2.0f;
    }if(key[SDL_SCANCODE_A]){
        state->texture_x -= 2.0f;
    }if(key[SDL_SCANCODE_D]){
        state->texture_x += 2.0f;
    }

    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    SDL_FRect dst_rect;
    dst_rect.w = (float)state->texture_width;
    dst_rect.h = (float)state->texture_height;
    dst_rect.x = state->texture_x;
    dst_rect.y = state->texture_y;
    SDL_RenderTexture(state->renderer, state->texture, NULL, &dst_rect);
    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}

