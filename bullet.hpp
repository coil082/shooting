#pragma once
#include "creature.hpp"

enum BulletRotation{
    me,
    enemy
};

class Bullet:public Creature{
private:
    BulletRotation rotation;
    bool fliped;
public:
    Bullet(SDL_FRect rect_,BulletRotation rotation_);
    void move();
    void update_dead(int window_w,int window_h);
    bool GetFliped();
};