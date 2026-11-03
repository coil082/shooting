#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <algorithm>
#include <memory>

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
        float w;
        float h;
        bool alive = true;

        Ballet(float x, float y,float w,float h, BalletRotation rotation):x(x),y(y),w(w),h(h),rotation(rotation){}

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

class Enemy{
    public:
        float x;
        float y;
        float w;
        float h;
        bool alive = true;
        Enemy(float x, float y,float w,float h):x(x),y(y),w(w),h(h){}

        void move(){
            y += 0.75f;
        }
};

typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    int me_w;
    int me_h;
    float me_x;
    float me_y;
    std::vector<std::unique_ptr<Enemy>> enemies;
    SDL_Texture *enemy_texture;
    std::vector<std::unique_ptr<Ballet>> ballets;
    SDL_Texture *ballet_texture;
    int ballet_cooldown;
    SDL_Texture *life_texture;
    int life;
}AppState;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    AppState *state = new AppState();
    state->window = NULL;
    state->renderer = NULL;
    state->texture = NULL;
    state->enemy_texture = NULL;
    state->ballet_texture = NULL;
    state->life_texture = NULL;
    state->ballet_cooldown = 60;
    state->enemies.push_back(std::make_unique<Enemy>(200.0f,50.0f,60.0f,60.0f));
    state->enemies.push_back(std::make_unique<Enemy>(400.0f,50.0f,60.0f,60.0f));
    state->enemies.push_back(std::make_unique<Enemy>(600.0f,50.0f,60.0f,60.0f));
    state->life = 5;
    SDL_Surface *surface = NULL;
    SDL_Surface *enemy_surface = NULL;
    SDL_Surface *ballet_surface = NULL;
    SDL_Surface *life_surface = NULL;
    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("Hello World", 800, 600, 0, &state->window, &state->renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    surface = SDL_LoadPNG("/img/Me.png");
    enemy_surface = SDL_LoadPNG("/img/enemy.png");
    ballet_surface = SDL_LoadPNG("/img/ballet.png");
    life_surface = SDL_LoadPNG("/img/life.png");
    if(!surface){
        SDL_Log("Couldn't load png%s",SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if(!enemy_surface){
        SDL_Log("Couldn't load enemy png%s",SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if(!ballet_surface){
        SDL_Log("Couldn't load ballet png%s",SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if(!life_surface){
        SDL_Log("Couldn't load life png%s",SDL_GetError());
        return SDL_APP_FAILURE;
    }
    state->me_w= surface->w;
    state->me_h = surface->h;
    state->me_x = (800-state->me_w)/2.0f;
    state->me_y = (600-state->me_h)/2.0f;
    state->texture = SDL_CreateTextureFromSurface(state->renderer,surface);
    state->enemy_texture = SDL_CreateTextureFromSurface(state->renderer,enemy_surface);
    state->ballet_texture = SDL_CreateTextureFromSurface(state->renderer,ballet_surface);
    state->life_texture = SDL_CreateTextureFromSurface(state->renderer,life_surface);
    SDL_DestroySurface(ballet_surface);
    SDL_DestroySurface(enemy_surface);
    SDL_DestroySurface(surface);
    SDL_DestroySurface(life_surface);
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
        state->me_y -= 2.0f;
    }if(key[SDL_SCANCODE_S]){
        state->me_y += 2.0f;
    }if(key[SDL_SCANCODE_A]){
        state->me_x -= 2.0f;
    }if(key[SDL_SCANCODE_D]){
        state->me_x += 2.0f;
    }
    if(key[SDL_SCANCODE_RIGHT]&&state->ballet_cooldown<=0){
        state->ballets.push_back(std::make_unique<Ballet>(state->me_x+(state->me_w/2.0f)-10.0f,state->me_y,20.0f,20.0f,BalletRotation::right));
        state->ballet_cooldown = 60;
    }if(key[SDL_SCANCODE_LEFT]&&state->ballet_cooldown<=0){
        state->ballets.push_back(std::make_unique<Ballet>(state->me_x+(state->me_w/2.0f)-10.0f,state->me_y,20.0f,20.0f,BalletRotation::left));
        state->ballet_cooldown = 60;
    }if(key[SDL_SCANCODE_UP]&&state->ballet_cooldown<=0){
        state->ballets.push_back(std::make_unique<Ballet>(state->me_x+(state->me_w/2.0f)-10.0f,state->me_y,20.0f,20.0f,BalletRotation::center));
        state->ballet_cooldown = 60;
    }

    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);

    for(auto& ballet: state->ballets){
        ballet->move();
        SDL_FRect ballet_rect;
        ballet_rect.w = ballet->w;
        ballet_rect.h = ballet->h;
        ballet_rect.x = ballet->x;
        ballet_rect.y = ballet->y;
        SDL_RenderTexture(state->renderer, state->ballet_texture, NULL,&ballet_rect);
    }
    for(auto& enemy: state->enemies){
        if(!enemy->alive){continue;}
        enemy->move();

        for(auto& ballet:state->ballets){
            if(enemy->x<ballet->x+ballet->w&&enemy->x+enemy->w>ballet->x&&enemy->y<ballet->y+ballet->h&&enemy->y+enemy->h>ballet->y){
                enemy->alive = false;
                ballet->alive = false;
                break;
            }
        }

        if(enemy->x<state->me_x+state->me_w&&enemy->x+enemy->w>state->me_x&&enemy->y<state->me_y+state->me_h&&enemy->y+enemy->h>state->me_y){
            enemy->alive = false;
            state->life--;
        }

        SDL_FRect enemy_rect;
        enemy_rect.w = enemy->w;
        enemy_rect.h = enemy->h;
        enemy_rect.x = enemy->x;
        enemy_rect.y = enemy->y;
        SDL_RenderTexture(state->renderer, state->enemy_texture, NULL,&enemy_rect);
    }
    std::erase_if(state->enemies,[](const auto &enemy){return !enemy->alive;});
    std::erase_if(state->ballets,[](const auto &ballet){return !ballet->alive;});

    SDL_FRect dst_rect;
    dst_rect.w = (float)state->me_w;
    dst_rect.h = (float)state->me_h;
    dst_rect.x = state->me_x;
    dst_rect.y = state->me_y;

    for(int i=0;i<state->life;i++){
        SDL_FRect life_rect;
        life_rect.w = 50.0f;
        life_rect.h = 50.0f;
        life_rect.x = 10.0f + i * 60.0f;
        life_rect.y = 10.0f;
        SDL_RenderTexture(state->renderer, state->life_texture, NULL,&life_rect);
    }

    SDL_RenderTexture(state->renderer, state->texture, NULL, &dst_rect);
    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if(appstate){
        AppState *state = (AppState *)appstate;
        if(state->texture){
            SDL_DestroyTexture(state->texture);
        }
        if(state->enemy_texture){
            SDL_DestroyTexture(state->enemy_texture);
        }
        if(state->ballet_texture){
            SDL_DestroyTexture(state->ballet_texture);
        }
        if(state->renderer){
            SDL_DestroyRenderer(state->renderer);
        }
        if(state->window){
            SDL_DestroyWindow(state->window);
        }
        delete state;
    }
}

