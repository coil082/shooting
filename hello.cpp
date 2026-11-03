#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>

enum BalletRotation{
    center,
    left,
    right
};

class Ballet{
    private:
        BalletRotation rotation;
    public:
        float x;
        float y;

        Ballet(float x, float y, BalletRotation rotation):x(x),y(y),rotation(rotation){}

        void move(){
            switch(rotation){
                case BalletRotation::center:
                    y -= 2.0f;
                    break;
                case BalletRotation::left:
                    x -= 2.0f;
                    y -= 2.0f;
                    break;
                case BalletRotation::right:
                    x += 2.0f;
                    y -= 2.0f;
                    break;
            }
        }
};
typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    int texture_width ;
    int texture_height;
    float texture_x;
    float texture_y;
    std::vector<Ballet> ballets;
    SDL_Texture *ballet_texture;
    int ballet_cooldown;
}AppState;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    AppState *state = (AppState *)SDL_malloc(sizeof(AppState));
    state->window = NULL;
    state->renderer = NULL;
    state->texture = NULL;
    state->ballet_texture = NULL;
    state->ballet_cooldown = 60;
    SDL_Surface *surface = NULL;
    SDL_Surface *ballet_surface = NULL;
    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("Hello World", 800, 600, 0, &state->window, &state->renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    surface = SDL_LoadPNG("/img/Me.png");
    ballet_surface = SDL_LoadPNG("/img/ballet.png");
    if(!surface){
        SDL_Log("Couldn't load png%s",SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if(!ballet_surface){
        SDL_Log("Couldn't load ballet png%s",SDL_GetError());
        return SDL_APP_FAILURE;
    }
    state->texture_width = surface->w;
    state->texture_height = surface->h;
    state->texture_x = (800-state->texture_width)/2.0f;
    state->texture_y = (600-state->texture_height)/2.0f;
    state->texture = SDL_CreateTextureFromSurface(state->renderer,surface);
    state->ballet_texture = SDL_CreateTextureFromSurface(state->renderer,ballet_surface);
    SDL_DestroySurface(ballet_surface);
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
    state->ballet_cooldown--;
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
    if(key[SDL_SCANCODE_RIGHT]&&state->ballet_cooldown<=0){
        state->ballets.push_back(Ballet(state->texture_x+state->texture_width/2.0f,state->texture_y,BalletRotation::right));
        state->ballet_cooldown = 60;
    }if(key[SDL_SCANCODE_LEFT]&&state->ballet_cooldown<=0){
        state->ballets.push_back(Ballet(state->texture_x+state->texture_width/2.0f,state->texture_y,BalletRotation::left));
        state->ballet_cooldown = 60;
    }if(key[SDL_SCANCODE_UP]&&state->ballet_cooldown<=0){
        state->ballets.push_back(Ballet(state->texture_x+state->texture_width/2.0f,state->texture_y,BalletRotation::center));
        state->ballet_cooldown = 60;
    }

    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);

    for(Ballet& ballet: state->ballets){
        ballet.move();
        SDL_FRect ballet_rect;
        ballet_rect.w = 20.0f;
        ballet_rect.h = 20.0f;
        ballet_rect.x = ballet.x;
        ballet_rect.y = ballet.y;
        SDL_RenderTexture(state->renderer, state->ballet_texture, NULL,&ballet_rect);
    }

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

