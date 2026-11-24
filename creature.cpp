#include "creature.hpp"

void Creature::move(){
    rect.y+=0.75f;
};
bool Creature::isAlive()const{
    return alive;
};
void Creature::kill(){
    alive=false;
};
void Creature::update_dead(int window_w,int window_h){
    if(rect.y>window_h||rect.x<0||rect.x>window_w){
        alive=false;
    }
};
Creature::Creature(SDL_FRect rect_):rect(rect_){}