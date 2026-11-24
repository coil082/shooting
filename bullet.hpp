#pragma once
#include "creature.hpp"

enum BulletRotation{
    center,
    left,
    right
};

class Bullet:public Creature{
private:
    BulletRotation rotation;
public:
    Bullet(SDL_FRect rect_,BulletRotation rotation_);
    void move();
    void update_dead(int window_w,int window_h);
};