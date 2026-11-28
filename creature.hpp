#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>

class Creature{
    protected:
        bool alive=true;
    public:
        SDL_FRect rect;
        void move();
        bool isAlive()const;
        void kill();
        void update_dead(int window_w,int window_h);
        Creature(SDL_FRect rect_);
};