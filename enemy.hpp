#pragma once
#include "creature.hpp"
#include "bullet.hpp"

class Enemy:public Creature{
    private:
        bool shoot;
    public:
        float cooldown;
        Enemy(SDL_FRect rect_,bool shoot_=false);
        bool GetShoot();
        void shootBullet(std::vector<Bullet>&);
};