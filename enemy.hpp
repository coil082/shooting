#pragma once
#include "creature.hpp"

class Enemy:public Creature{
    public:
        Enemy(SDL_FRect rect_);
};