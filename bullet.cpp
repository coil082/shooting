#include "bullet.hpp"

Bullet::Bullet(SDL_FRect rect_,BulletRotation rotation_):Creature(rect_),rotation(rotation_){};
void Bullet::move(){
    switch(rotation){
            case BulletRotation::center:
                rect.y -=2.0f;
                break;
            case BulletRotation::left:
                rect.x -= 2.0f;
                rect.y -= 2.0f;
                break;
            case BulletRotation::right:
                rect.x += 2.0f;
                rect.y -= 2.0f;
                break;
        }
};

void Bullet::update_dead(int window_w,int window_h){
   if(rect.y<0||rect.y>window_h||rect.x<0||rect.x>window_w){
        alive=false;
    }
};