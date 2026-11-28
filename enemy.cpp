#include "enemy.hpp"

Enemy::Enemy(SDL_FRect rect_,bool shoot_):Creature(rect_),shoot(shoot_){};
void Enemy::shootBullet(std::vector<Bullet>& enemy_bullets){
    enemy_bullets.push_back(Bullet({rect.x+rect.w/2-5,rect.y+rect.h,10,10},enemy));
};
bool Enemy::GetShoot(){
    return shoot;
};